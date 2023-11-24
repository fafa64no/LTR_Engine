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


static RenderInterface::Shader* menuShader;
static RenderInterface::Shader* lightShader;

static RenderInterface::Texture* ltrTexture;
static RenderInterface::Texture* woodTexture;
static RenderInterface::Texture* awesomeTexture;
static unsigned int 
    cubes_VBO,cubes_VAO,cubes_EBO,
    lights_VBO,lights_VAO,lights_EBO;

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
    //glClearColor(0.1f,0.1f,0.12f,1);
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
void gl_render_3D_layer(glm::mat4 viewMat,glm::mat4 projMat){
    glActiveTexture(GL_TEXTURE0);
    faridTexture->use();

    testShader->use();
    testShader->setInt("textureUsed1",0);
    testShader->setInt("textureUsed2",1);

    testShader->setVec3("viewPos",RenderInterface::renderData->currentCamera->camPos);

    testShader->setVec3("ambientLight",gameData->currentZone->ambientLight);
    testShader->setVec3("lightPos",glm::vec3(1.0,1.0,1.0));
    testShader->setVec3("diffuseLightColor",glm::vec3(1.0,1.0,1.0));
    testShader->setFloat("diffuseLightStrength",1.3f);
    testShader->setFloat("diffuseLightRange",15.0f);

    testShader->setVec3("material.ambient",metal.ambient);
    testShader->setVec3("material.diffuse",metal.diffuse);
    testShader->setVec3("material.specular",metal.specular);
    testShader->setInt("material.shininess",metal.shininess);

    //Scene vars
    static float modelAngle=0.0;

    testShader->setMat4("view",viewMat);
    testShader->setMat4("proj",projMat);

    for(int i=0;i<RenderInterface::renderData->nodeCount;i++){
        RenderInterface::renderData->nodes_to_render[i]->Draw(RenderInterface::renderData);
    }
}
// ############################################################################
void gl_render_3d_lights(glm::mat4 viewMat,glm::mat4 projMat){
    lightShader->use();

    lightShader->setMat4("view",viewMat);
    lightShader->setMat4("proj",projMat);

    glm::mat4 modelMat=glm::mat4(1.0);
    modelMat=glm::translate(modelMat,glm::vec3(1.0,1.0,1.0));
    modelMat=glm::scale(modelMat,glm::vec3(0.2, 0.2, 0.2));
    lightShader->setMat4("model",modelMat);

    glBindVertexArray(lights_VAO);
    glDrawElements(GL_TRIANGLES,sizeof(light_indices),GL_UNSIGNED_INT,nullptr);
}
// ############################################################################
void gl_render(){
    glViewport(0,0,input->screenSize.x,input->screenSize.y);
    gl_clear();

    //View matrix
    RenderInterface::renderData->currentCamera->updateDir(input->mouseDir);
    glm::mat4 viewMat=RenderInterface::renderData->currentCamera->viewMat();
    //Projection matrix
    glm::mat4 projMat=glm::perspective(glm::radians(45.0f), (float)input->screenSize.x/(float)input->screenSize.y, 0.1f, 100.0f);

    gl_render_3D_layer(viewMat,projMat);
    gl_render_3d_lights(viewMat,projMat);
    gl_render_2D_layer();
}

// ############################################################################
//                            OpenGL Init Functions
// ############################################################################
void gl_shaders_init(BumpAllocator* persistentStorage){
    testShader=new RenderInterface::Shader("assets/shaders/test.vert","assets/shaders/test.frag",persistentStorage);

    //menuShader=new Shader("assets/shaders/menuShader.vert","assets/shaders/menuShader.frag",persistentStorage);
    lightShader=new RenderInterface::Shader("assets/shaders/lightShader.vert","assets/shaders/lightShader.frag",persistentStorage);
}
void gl_textures_init(){
    ltrTexture=new RenderInterface::Texture("assets/textures/LTR.png",GL_RGBA);
    faridTexture=new RenderInterface::Texture("assets/textures/farid.png",GL_RGBA);
    woodTexture=new RenderInterface::Texture("assets/textures/container.jpg",GL_RGB);
    awesomeTexture=new RenderInterface::Texture("assets/textures/awesomeface.png",GL_RGBA);
}
bool gl_init(BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
    load_gl_functions();
    glDebugMessageCallback(&gl_debug_callback,nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    //Init Shaders and Materials
    gl_shaders_init(persistentStorage);
    gl_materials_init();

    //Generate VAO, VBO and EBO for lights
    glGenVertexArrays(1,&lights_VAO);
    glGenBuffers(1,&lights_VBO);
    glGenBuffers(1,&lights_EBO);
    glBindVertexArray(lights_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,lights_VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(light_vertices),light_vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lights_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(light_indices),light_indices,GL_STATIC_DRAW);
    //Vertex attribs
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); 

    //Load textures
    gl_textures_init();

    //Init camera
    RenderInterface::renderData->currentCamera=new RenderInterface::Camera(
        glm::vec3(0.0,0.0,-3.0),
        glm::vec3(1.0,0.0,0.0),
        glm::vec3(0.0,1.0,0.0));
    return true;
}

