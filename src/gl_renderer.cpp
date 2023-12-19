#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "assets.h"
#include "input.h"
#include "zone.h"
#include "game.h"
#include "materials.h"
#include "scenes.h"
#include "gl_uniforms.h"

// ############################################################################
//                            OpenGL Constants
// ############################################################################
#include "test_models.h"
float frameQuadVertices[]={  
    //positions    //texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};
const unsigned int DIR_SHADOW_WIDTH=4096,DIR_SHADOW_HEIGHT=4096;

// ############################################################################
//                            OpenGL Structs
// ############################################################################
struct GLContext{
    GLuint programID;
    GLuint textureID;
    GLuint transformSBOID;
    GLuint screenSizeID;
};

// ############################################################################
//                            OpenGL Globals
// ############################################################################
static GLContext glContext;

static unsigned int 
    FBO,RBO,frameTexture,frameVAO,frameVBO,
    dirLightDepthMapFBO,dirLightDepthMap,
    matrixUBO;

// ############################################################################
//                            OpenGL Functions
// ############################################################################
static void APIENTRY gl_debug_callback(
                                GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* user){
    if (severity==GL_DEBUG_SEVERITY_LOW||
        severity==GL_DEBUG_SEVERITY_MEDIUM||
        severity==GL_DEBUG_SEVERITY_HIGH){
        SM_TRACE("gl_debug_callback:");
        SM_TRACE((char*)message);
        SM_ASSERT(false,"OpenGL error, exiting for safety...");
    }else{
        SM_TRACE((char*)message);
    }
}
void gl_clear(){
    glClearColor(0.8f,0.8f,0.6f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

// ############################################################################
//                            OpenGL Render Functions
// ############################################################################
void gl_render_2D_layer(){
    //menuShader->use();
}
// ############################################################################
void gl_render_3D_layer(){
    glBindBuffer(GL_UNIFORM_BUFFER,matrixUBO);
    using namespace RenderInterface;
    if(gameData->debugMode){
        gldb_uniformBuffer_update(
            renderData->currentCamera->projMat()*renderData->currentCamera->viewMat(),
            renderData->currentCamera->viewMat(),
            gameData->currentBiome->getLightSpaceMatrix(),
            gameData->currentBiome->sunDir,
            gameData->currentBiome->sunCol,
            gameData->currentBiome->ambientLight
        );
    }else{
        gl_uniformBuffer_update(
            renderData->currentCamera->projMat()*renderData->currentCamera->viewMat(),
            renderData->currentCamera->viewMat(),
            gameData->currentBiome->getLightSpaceMatrix(),
            gameData->currentBiome->sunDir,
            gameData->currentBiome->sunCol,
            gameData->currentBiome->ambientLight
        );
    }
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    for(int i=0;i<renderData->nodeCount;i++)renderData->nodes_to_render[i]->Draw(renderData);
}
// ############################################################################
void gl_render(){
    unsigned int width{(unsigned int)input->screenSize.x},height{(unsigned int)input->screenSize.y};
    unsigned int pixWidth{(unsigned int)width/RenderInterface::renderData->pixelation},pixHeight{(unsigned int)height/RenderInterface::renderData->pixelation};
    //Render shadow map
    static int shadi{0};
    if(shadi==0){
        glViewport(0,0,DIR_SHADOW_WIDTH,DIR_SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER,dirLightDepthMapFBO);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
        gameData->currentBiome->updateLightSpaceMatrix();
        for(int i=0;i<RenderInterface::renderData->nodeCount;i++)RenderInterface::renderData->nodes_to_render[i]->CastShadow(*dirShadowShader,RenderInterface::renderData);
        glBindFramebuffer(GL_FRAMEBUFFER,0);
        shadi++;
    }
    //Render view
    glViewport(0,0,pixWidth,pixHeight);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glEnable(GL_DEPTH_TEST);
    gl_clear();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,dirLightDepthMap);
    RenderInterface::renderData->currentCamera->updateRatio(input->screenRatio);
    RenderInterface::renderData->currentCamera->updateViewMat();
    RenderInterface::renderData->currentCamera->updateProjMat();
    RenderInterface::renderData->viewMat=RenderInterface::renderData->currentCamera->viewMat();
    RenderInterface::renderData->projMat=RenderInterface::renderData->currentCamera->projMat();
    gl_render_3D_layer();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    //Post processing
    glViewport(0,0,width,height);
    gl_clear();
    frameQuadShader->use();
    frameQuadShader->setVec2("screenRatio",glm::vec2((float)pixWidth/input->maxScreenSize.x,(float)pixHeight/input->maxScreenSize.y));
    glBindVertexArray(frameVAO);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D,frameTexture);
    glDrawArrays(GL_TRIANGLES,0,6);
    //Render UI
    gl_render_2D_layer();
}

// ############################################################################
//                            OpenGL Init Functions
// ############################################################################
void gl_shaders_init(BumpAllocator* persistentStorage){
    using namespace RenderInterface;
    // 3D shaders
    //testShader      =new Shader("assets/shaders/test.vert"              ,"assets/shaders/test.frag"             ,persistentStorage);
    diffuseShader   =new Shader("assets/shaders/diffuseShader.vert"     ,"assets/shaders/diffuseShader.frag"    ,persistentStorage);

    //shaders_3D.push_back(testShader);
    shaders_3D.push_back(diffuseShader);

    // 2D shaders
    frameQuadShader =new Shader("assets/shaders/frameQuadShader.vert"   ,"assets/shaders/frameQuadShader.frag"  ,persistentStorage);

    // Shadow shaders
    dirShadowShader =new Shader("assets/shaders/dirShadowShader.vert"   ,"assets/shaders/dirShadowShader.frag"  ,persistentStorage);
}
void gl_textures_init(){
    using namespace RenderInterface;
    faridTexture=new Texture("assets/textures/farid.png",GL_RGBA);
    groundTexture=new Texture("assets/textures/ground.png",GL_RGB);
    building1Texture=new Texture("assets/textures/building1.png",GL_RGB);
    building2Texture=new Texture("assets/textures/building2.png",GL_RGB);
    building3Texture=new Texture("assets/textures/farid.png",GL_RGBA);
}
bool gl_init(BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
    load_gl_functions();
    glDebugMessageCallback(&gl_debug_callback,nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Init Materials, Textures and Shaders
    gl_materials_init();
    gl_textures_init();
    gl_shaders_init(persistentStorage);

    //Max screen size
    {
        char buff[64]{0};
        platform_get_screen_size(&input->maxScreenSize.x,&input->maxScreenSize.y);
        sprintf(buff,"----maxScreenSize:\n\t%d\t%d\n",input->maxScreenSize.x,input->maxScreenSize.y);
        SM_TRACE(buff);
    }

    //Frame buffers
    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    glGenTextures(1,&frameTexture);
    glBindTexture(GL_TEXTURE_2D,frameTexture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,input->maxScreenSize.x,input->maxScreenSize.y,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,frameTexture,0);
    glGenRenderbuffers(1,&RBO);
    glBindRenderbuffer(GL_RENDERBUFFER,RBO); 
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,input->maxScreenSize.x,input->maxScreenSize.y);  
    glBindRenderbuffer(GL_RENDERBUFFER,0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,RBO);
    glGenVertexArrays(1,&frameVAO);
    glGenBuffers(1,&frameVBO);
    glBindVertexArray(frameVAO);
    glBindBuffer(GL_ARRAY_BUFFER,frameVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(frameQuadVertices),&frameQuadVertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
    glBindVertexArray(0);

    SM_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE,"Scene framebuffer incomplete");
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    glGenFramebuffers(1,&dirLightDepthMapFBO);
    glGenTextures(1,&dirLightDepthMap);
    glBindTexture(GL_TEXTURE_2D,dirLightDepthMap);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,DIR_SHADOW_WIDTH,DIR_SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
    float borderShadowColor[4]{1.0f,1.0f,1.0f,1.0f};
    glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderShadowColor);
    glBindFramebuffer(GL_FRAMEBUFFER,dirLightDepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,dirLightDepthMap,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0); 

    SM_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER)==GL_FRAMEBUFFER_COMPLETE,"Depthmap framebuffer incomplete");
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    //Uniform buffers
    gl_uniformBuffer_setup();
    for(int i=0;i<RenderInterface::shaders_3D.size();i++)RenderInterface::shaders_3D[i]->setUniformBlock("MatsAndVecs",0);
    glGenBuffers(1,&matrixUBO);
    glBindBuffer(GL_UNIFORM_BUFFER,matrixUBO);
    glBufferData(GL_UNIFORM_BUFFER,uniformBufferSize,NULL,GL_DYNAMIC_DRAW); // allocate 60*sizeof(float) bytes of memory
    glBindBufferRange(GL_UNIFORM_BUFFER,0,matrixUBO,0,uniformBufferSize);
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    RenderInterface::renderData->pixelation=1;
    return true;
}

