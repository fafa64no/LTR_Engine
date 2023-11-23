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
const glm::vec3 cubePositions[] = {
    glm::vec3(-2.0f, -1.0f,  0.0f), 
    glm::vec3( 2.0f,  1.0f,  0.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -6.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -5.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

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


static Shader* menuShader;
static Shader* lightShader;

static Texture* ltrTexture;
static Texture* woodTexture;
static Texture* awesomeTexture;
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
    glClearColor(0.1f,0.1f,0.12f,1);
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
//    //Cubes model matrix
//    for(unsigned int i=0;i<10;i++){
//        glm::mat4 modelMat=glm::mat4(1.0);
//        modelMat=glm::translate(modelMat,cubePositions[i]);
//        modelMat=glm::rotate(modelMat,glm::radians(modelAngle+i*20),glm::vec3(0.5,1.0,1.0));
//        modelMat=glm::scale(modelMat,glm::vec3(0.5,0.5,0.5));
//
//        testShader->setMat4("model",modelMat);
//
//        glm::mat4 normalMat=glm::transpose(glm::inverse(viewMat*modelMat));
//        testShader->setMat4("normalMat",normalMat);
//
//        glBindVertexArray(cubes_VAO);
//        glDrawElements(GL_TRIANGLES,sizeof(cube_indices),GL_UNSIGNED_INT,nullptr);
//    }
//    if(!gameData->is_paused)modelAngle+=0.5;
//    //Ground model matrix
//    glActiveTexture(GL_TEXTURE0);
//    woodTexture->use();
//    glm::mat4 modelMat=glm::mat4(1.0);
//    modelMat=glm::translate(modelMat,glm::vec3(0.0,-13.0,-5.0));
//    modelMat=glm::scale(modelMat,glm::vec3(20.0,20.0,20.0));
//
//    testShader->setMat4("model",modelMat);
//
//    glm::mat4 normalMat=glm::transpose(glm::inverse(viewMat*modelMat));
//    testShader->setMat4("normalMat",normalMat);
//
//    glBindVertexArray(cubes_VAO);
//    glDrawElements(GL_TRIANGLES,sizeof(cube_indices),GL_UNSIGNED_INT,nullptr);
//
//    glActiveTexture(GL_TEXTURE0);
//    woodTexture->use();
//    RenderInterface::testScene->Draw(*testShader);

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
    testShader=new Shader("assets/shaders/test.vert","assets/shaders/test.frag",persistentStorage);

    //menuShader=new Shader("assets/shaders/menuShader.vert","assets/shaders/menuShader.frag",persistentStorage);
    lightShader=new Shader("assets/shaders/lightShader.vert","assets/shaders/lightShader.frag",persistentStorage);
}
void gl_textures_init(BumpAllocator* persistentStorage){
    ltrTexture=new Texture("assets/textures/LTR.png",persistentStorage,GL_RGBA);
    faridTexture=new Texture("assets/textures/farid.png",persistentStorage,GL_RGBA);
    woodTexture=new Texture("assets/textures/container.jpg",persistentStorage,GL_RGB);
    awesomeTexture=new Texture("assets/textures/awesomeface.png",persistentStorage,GL_RGBA);
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

    //Generate VAO, VBO and EBO for cubes
    glGenVertexArrays(1,&cubes_VAO);
    glGenBuffers(1,&cubes_VBO);
    glGenBuffers(1,&cubes_EBO);
    glBindVertexArray(cubes_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,cubes_VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cube_with_normals_vertices),cube_with_normals_vertices,GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,cubes_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cube_with_normals_indices),cube_with_normals_indices,GL_DYNAMIC_DRAW);
    //Vertex attribs
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,11*sizeof(float),(void*)(8*sizeof(float)));
    glEnableVertexAttribArray(3);

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
    gl_textures_init(persistentStorage);

    //Init camera
    RenderInterface::renderData->currentCamera=new RenderInterface::Camera(
        glm::vec3(0.0,0.0,-3.0),
        glm::vec3(1.0,0.0,0.0),
        glm::vec3(0.0,1.0,0.0));
    return true;
}

