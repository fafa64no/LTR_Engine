#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "assets.h"
#include "input.h"
#include "zone.h"
#include "game.h"
#include "materials.h"
#include "scenes.h"

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
    dirLightDepthMapFBO,dirLightDepthMap;

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
    glClearColor(0.8f,0.8f,0.8f,1);
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
    glActiveTexture(GL_TEXTURE0);
    faridTexture->use();

    testShader->use();
    testShader->setInt("textureUsed1",0);
    testShader->setInt("textureUsed2",1);

    testShader->setVec3("ambientLight",gameData->currentBiome->ambientLight);
    testShader->setVec3("lightPos",glm::vec3(1.0,1.0,1.0));
    testShader->setVec3("diffuseLightColor",glm::vec3(1.0,1.0,1.0));
    testShader->setFloat("diffuseLightStrength",1.3f);
    testShader->setFloat("diffuseLightRange",15.0f);

    testShader->setVec3("material.ambient",metal.ambient);
    testShader->setVec3("material.diffuse",metal.diffuse);
    testShader->setVec3("material.specular",metal.specular);
    testShader->setInt("material.shininess",metal.shininess);
}
// ############################################################################
void gl_render(){
    unsigned int width{(unsigned int)input->screenSize.x},height{(unsigned int)input->screenSize.y};
    unsigned int pixWidth{(unsigned int)width/RenderInterface::renderData->pixelation},pixHeight{(unsigned int)height/RenderInterface::renderData->pixelation};
    //Render shadow map
    glViewport(0,0,DIR_SHADOW_WIDTH,DIR_SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER,dirLightDepthMapFBO);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    gameData->currentBiome->updateLightSpaceMatrix();
    for(int i=0;i<RenderInterface::renderData->nodeCount;i++)RenderInterface::renderData->nodes_to_render[i]->CastShadow(*dirShadowShader,RenderInterface::renderData);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
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
    for(int i=0;i<RenderInterface::renderData->nodeCount;i++)RenderInterface::renderData->nodes_to_render[i]->Draw(RenderInterface::renderData);
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
    testShader=new RenderInterface::Shader("assets/shaders/test.vert","assets/shaders/test.frag",persistentStorage);
    frameQuadShader=new RenderInterface::Shader("assets/shaders/frameQuadShader.vert","assets/shaders/frameQuadShader.frag",persistentStorage);
    diffuseShader=new RenderInterface::Shader("assets/shaders/diffuseShader.vert","assets/shaders/diffuseShader.frag",persistentStorage);
    dirShadowShader=new RenderInterface::Shader("assets/shaders/dirShadowShader.vert","assets/shaders/dirShadowShader.frag",persistentStorage);
}
void gl_textures_init(){
    faridTexture=new RenderInterface::Texture("assets/textures/farid.png",GL_RGBA);
    groundTexture=new RenderInterface::Texture("assets/textures/ground.png",GL_RGB);
    building1Texture=new RenderInterface::Texture("assets/textures/building1.png",GL_RGB);
    building2Texture=new RenderInterface::Texture("assets/textures/building2.png",GL_RGB);
    building3Texture=new RenderInterface::Texture("assets/textures/farid.png",GL_RGBA);
}
bool gl_init(BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
    load_gl_functions();
    glDebugMessageCallback(&gl_debug_callback,nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Init Shaders and Materials
    gl_shaders_init(persistentStorage);
    gl_materials_init();
    //Load textures
    gl_textures_init();

    //Frame buffers
    input->maxScreenSize=glm::ivec2(1980,1080);
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

    RenderInterface::renderData->pixelation=4;
    return true;
}

