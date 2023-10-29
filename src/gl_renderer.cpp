#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "assets.h"
#include "input.h"



// ############################################################################
//                            OpenGL Constants
// ############################################################################
const char* TEXTURE_PATH_LOADING="assets/textures/LTR.png";
float cube_vertices[]={
    //Positions              //Colors                //Texture position    
    -0.5f,  -0.5f,  -0.5f,   1.0f,   0.0f,   0.0f,   1.0f,   1.0f,
    -0.5f,  -0.5f,   0.5f,   1.0f,   1.0f,   0.0f,   1.0f,   0.0f,
    -0.5f,   0.5f,  -0.5f,   1.0f,   1.0f,   1.0f,   0.0f,   0.0f,
    -0.5f,   0.5f,   0.5f,   0.0f,   1.0f,   1.0f,   0.0f,   1.0f,
     0.5f,  -0.5f,  -0.5f,   0.0f,   0.0f,   1.0f,   1.0f,   1.0f,
     0.5f,  -0.5f,   0.5f,   0.0f,   0.0f,   0.0f,   1.0f,   0.0f,
     0.5f,   0.5f,  -0.5f,   1.0f,   0.0f,   1.0f,   0.0f,   0.0f,
     0.5f,   0.5f,   0.5f,   1.0f,   1.0f,   0.0f,   0.0f,   1.0f
};
unsigned int cube_indices[]={
    0,1,2,
    1,2,3,
    4,5,6,
    5,6,7,
    1,3,5,
    3,5,7,
    0,2,4,
    2,4,6,
    0,1,4,
    1,4,5,
    2,3,6,
    3,6,7
};
float rectangle_vertices[] = {
    // positions          // colors           // texture coords
    0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};
unsigned int rectangle_indices[] = {  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
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
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
void gl_shaders_init(BumpAllocator* transientStorage){
    testShader=new Shader("assets/shaders/test.vert","assets/shaders/test.frag",transientStorage);
    quadShader=new Shader("assets/shaders/quad.vert","assets/shaders/quad.frag",transientStorage);
}
void gl_textures_init(BumpAllocator* transientStorage){
    ltrTexture=new Texture("assets/textures/LTR.png",transientStorage);
    faridTexture=new Texture("assets/textures/farid.png",transientStorage);
    woodTexture=new Texture("assets/textures/container.jpg",transientStorage);
    awesomeTexture=new Texture("assets/textures/awesomeface.png",transientStorage);
}

void gl_render(){
    glViewport(0,0,input->screenSizeX,input->screenSizeY);
    //Vec2 screenSize={(float)input->screenSizeX,(float)input->screenSizeY};
    //glUniform2fv(glContext.screenSizeID,1,&screenSize.x);
    gl_clear();
    faridTexture->use();
    testShader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,sizeof(rectangle_indices),GL_UNSIGNED_INT,0);
}


bool gl_init(BumpAllocator* transientStorage){
    load_gl_functions();
    glDebugMessageCallback(&gl_debug_callback,nullptr);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    //Init Shaders
    gl_shaders_init(transientStorage);

    //Generate VAO, VBO and EBO
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(rectangle_vertices),rectangle_vertices,GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(rectangle_indices),rectangle_indices,GL_DYNAMIC_DRAW);

    //Vertex attribs
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    //Load textures
    gl_textures_init(transientStorage);

    return true;
}









