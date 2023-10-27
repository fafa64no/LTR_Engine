#include "gl_renderer.h"
#include "LTR_Engine_lib.h"

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
bool gl_init(BumpAllocator* transientStorage){
    load_gl_functions();
    glDebugMessageCallback(&gl_debug_callback,nullptr);
    //Enables
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_DEPTH_TEST);
    //Shaders import
    GLuint vertShaderID=glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShaderID=glCreateShader(GL_FRAGMENT_SHADER);
    int fileSize=0;
    char* vertShader=read_file("assets/shaders/quad.vert",&fileSize,transientStorage);
    char* fragShader=read_file("assets/shaders/quad.frag",&fileSize,transientStorage);
    if (!vertShader||!fragShader){
        SM_ASSERT(false,"failed to load shaders");
        return false;
    }
    //Implement shaders
    glShaderSource(vertShaderID,1,&vertShader,0);
    glShaderSource(fragShaderID,1,&fragShader,0);
    glCompileShader(vertShaderID);
    glCompileShader(fragShaderID);
    //Test shaders compiling
    {
        int success;
        char shaderLog[2048]={};
        glGetShaderiv(vertShaderID,GL_COMPILE_STATUS,&success);
        if (!success){
            glGetShaderInfoLog(vertShaderID,2048,0,shaderLog);
            SM_ASSERT(false,"Failed to compile Vertex Shaders %s",shaderLog);
        }
    }{
        int success;
        char shaderLog[2048]={};
        glGetShaderiv(fragShaderID,GL_COMPILE_STATUS,&success);
        if (!success){
            glGetShaderInfoLog(fragShaderID,2048,0,shaderLog);
            SM_ASSERT(false,"Failed to compile Fragment Shaders %s",shaderLog);
        }
    }
    //Link shaders
    glContext.programID=glCreateProgram();
    glAttachShader(glContext.programID,vertShaderID);
    glAttachShader(glContext.programID,fragShaderID);
    glLinkProgram(glContext.programID);
    //Clean up
    glDetachShader(glContext.programID,vertShaderID);
    glDetachShader(glContext.programID,fragShaderID);
    //Necessary because people said so idk
    GLuint VAO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    //Texture LTR
    gl_load_texture(TEXTURE_PATH_LOADING);
    //Transform storage buffer
    {
        glGenBuffers(1,&glContext.transformSBOID);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,0,glContext.transformSBOID);
        glBufferData(GL_SHADER_STORAGE_BUFFER,sizeof(Transform)* MAX_TRANSFORMS,renderData->transforms,GL_DYNAMIC_DRAW);
    }
    //Uniforms
    {
        glContext.screenSizeID=glGetUniformLocation(glContext.programID,"screenSize");
    }
    //Depth testing
    glDepthFunc(GL_GREATER);
    glUseProgram(glContext.programID);
    //Render
    glClearColor(0.0f/255.0f,0.0f/255.0f,0.0f/255.0f,1);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glViewport(0,0,input->screenSizeX,input->screenSizeY);
    glDrawArrays(GL_TRIANGLES,0,6);
    return true;
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
    //Load texture
    int width,height,channels;
    char* data=(char*)stbi_load(path,&width,&height,&channels,4);
    if (!data){
        SM_ASSERT(false,"Failed to load texture");
        return false;
    }
    //Set the texture 
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
    //Free memory
    stbi_image_free(data);
    return true;
}











