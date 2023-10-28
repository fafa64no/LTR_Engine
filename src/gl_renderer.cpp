#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "assets.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ############################################################################
//                            OpenGL Constants
// ############################################################################
const char* TEXTURE_PATH_LOADING="assets/textures/LTR.png";

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
        SM_ASSERT(false,"OpenGL Error: %s",message);
    }else{
        SM_TRACE((char*)message);
    }
}

void gl_shaders_init(BumpAllocator* transientStorage){
    //Shaders import
    testShader=new Shader("assets/shaders/test.vert","assets/shaders/test.frag",transientStorage);
    quadShader=new Shader("assets/shaders/quad.vert","assets/shaders/quad.frag",transientStorage);
}

void gl_render(){
    //Reset window
    glClearColor(0,0,0,1);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,input->screenSizeX,input->screenSizeY);
    //Copy screen size to the GPU
    Vec2 screenSize={(float)input->screenSizeX,(float)input->screenSizeY};
    glUniform2fv(glContext.screenSizeID,1,&screenSize.x);
    //Opaque Objects
    {
        glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Transform)* MAX_TRANSFORMS,renderData->transforms,GL_DYNAMIC_DRAW);
        glDrawArraysInstanced(GL_TRIANGLES,0,6,renderData->transformCount);
        //Reset
        renderData->transformCount=0;
    }
}

bool gl_load_texture(const char* path){
    //Load texture from file
    int width,height,channels;
    char* data=(char*)stbi_load(path,&width,&height,&channels,4);
    if (!data){
        SM_ASSERT(false,"Failed to load texture");
        return false;
    }
    //Turn it into openGL texture
    glGenTextures(1,&glContext.textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,glContext.textureID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_SRGB8_ALPHA8,
                width,height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data);
    //Clean up
    stbi_image_free(data);
    return true;
}

bool gl_init(BumpAllocator* transientStorage){
    load_gl_functions();
    glDebugMessageCallback(&gl_debug_callback,nullptr);
    gl_shaders_init(transientStorage);
    //Enables
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_DEPTH_TEST);
    //Generate Vertex Array Object
    GLuint VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    //Load Shaders and Textures
    quadShader->use();
    glContext.programID=quadShader->programID;
    gl_load_texture(TEXTURE_PATH_LOADING);
    SM_TRACE("test 4");
    //Transform storage buffer
    {
        glGenBuffers(1,&glContext.transformSBOID);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,0,glContext.transformSBOID);
        glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Transform)*MAX_TRANSFORMS,renderData->transforms,GL_DYNAMIC_DRAW);
    }
    //Uniforms
    {
        glContext.screenSizeID=glGetUniformLocation(glContext.programID,"screenSize");
    }
    //Depth testing
    glDepthFunc(GL_GREATER);
    //Set the program to use
    glUseProgram(glContext.programID);
    SM_TRACE("test 5");
    //Render
    glClearColor(0.0f/255.0f,0.0f/255.0f,0.0f/255.0f,1);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,input->screenSizeX,input->screenSizeY);
    glDrawArrays(GL_TRIANGLES,0,6);
    return true;
}









