#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "assets.h"
#include "input.h"

// ############################################################################
//                            OpenGL Constants
// ############################################################################
#include "test_models.h"
const glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
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
static Shader* testShader;
static Shader* quadShader;
static Texture* ltrTexture;
static Texture* faridTexture;
static Texture* woodTexture;
static Texture* awesomeTexture;
static unsigned int VBO, VAO, EBO;

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
void gl_shaders_init(BumpAllocator* transientStorage){
    testShader=new Shader("assets/shaders/test.vert","assets/shaders/test.frag",transientStorage);
    quadShader=new Shader("assets/shaders/quad.vert","assets/shaders/quad.frag",transientStorage);
}
void gl_textures_init(BumpAllocator* transientStorage){
    ltrTexture=new Texture("assets/textures/LTR.png",transientStorage,GL_RGBA);
    faridTexture=new Texture("assets/textures/farid.png",transientStorage,GL_RGBA);
    woodTexture=new Texture("assets/textures/container.jpg",transientStorage,GL_RGB);
    awesomeTexture=new Texture("assets/textures/awesomeface.png",transientStorage,GL_RGBA);
}

void gl_render(){
    glViewport(0,0,input->screenSize.x,input->screenSize.y);
    gl_clear();

    glActiveTexture(GL_TEXTURE0);
    faridTexture->use();
    glActiveTexture(GL_TEXTURE1);
    awesomeTexture->use();

    testShader->use();
    testShader->setInt("textureUsed1",0);
    testShader->setInt("textureUsed2",1);

    //Scene vars
    static float modelAngle=0.0;
    glm::vec3 transformPos=glm::vec3(0.0,0.0,2.5);

    //View matrix
    renderData->currentCamera->updateDir(input->mouseDir);
    glm::mat4 viewMat=renderData->currentCamera->viewMat();

    //Projection matrix
    glm::mat4 projMat=glm::perspective(glm::radians(45.0f), (float)input->screenSize.x/(float)input->screenSize.y, 0.1f, 100.0f);

    testShader->setMat4("view",viewMat);
    testShader->setMat4("proj",projMat);

    //Model matrix
    for(unsigned int i=0;i<10;i++){
        glm::mat4 modelMat=glm::mat4(1.0);
        modelMat=glm::translate(modelMat, cubePositions[i]);
        modelMat=glm::rotate(modelMat,glm::radians(modelAngle+i*20),glm::vec3(0.5, 1.0, 1.0));
        modelMat=glm::scale(modelMat,glm::vec3(0.5, 0.5, 0.5));

        testShader->setMat4("model",modelMat);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,sizeof(cube_indices),GL_UNSIGNED_INT,nullptr);
    }
    modelAngle+=5.5;
}


bool gl_init(BumpAllocator* transientStorage){
    load_gl_functions();
    glDebugMessageCallback(&gl_debug_callback,nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    //Init Shaders
    gl_shaders_init(transientStorage);

    //Generate VAO, VBO and EBO
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(cube_vertices),cube_vertices,GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(cube_indices),cube_indices,GL_DYNAMIC_DRAW);

    //Vertex attribs
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    //Load textures
    gl_textures_init(transientStorage);

    //Init camera
    renderData->currentCamera=new Camera(
        glm::vec3(0.0,0.0,0-3.0),
        glm::vec3(1.0,0.0,0.0),
        glm::vec3(0.0,1.0,0.0));

    return true;
}









