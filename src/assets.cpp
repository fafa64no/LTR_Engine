#include "assets.h"
#include "gl_renderer.h"

Shader::Shader(char* vertexPath, char* fragmentPath,BumpAllocator* bumpAllocator){
// 1. retrieve the vertex/fragment source code from filePath
    SM_TRACE("Building shader");
    int vShaderFileSize,fShaderFileSize;
    const char* vShaderCode=read_file(vertexPath,&vShaderFileSize,bumpAllocator);
    const char* fShaderCode=read_file(fragmentPath,&fShaderFileSize,bumpAllocator);
    SM_ASSERT(vShaderCode,"Failed to read shader: %s",vertexPath);
    SM_ASSERT(fShaderCode,"Failed to read shader: %s",fragmentPath);
// 2. compile shaders
    SM_TRACE("Compiling shader");
    unsigned int vertexShader,fragmentShader;
    int success;

    //Vertex Shader
    vertexShader=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vShaderCode,NULL);
    glCompileShader(vertexShader);
    //Check compiling
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    SM_ASSERT(success,"Failed to compile Vertex Shader");

    //Fragment Shader
    fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fShaderCode,NULL);
    glCompileShader(fragmentShader);
    //Check compiling
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    SM_ASSERT(success,"Failed to compile Fragment Shader");

    //Shader programm
    programID=glCreateProgram();
    glAttachShader(programID,vertexShader);
    glAttachShader(programID,fragmentShader);
    glLinkProgram(programID);
    //Check linking
    glGetProgramiv(programID,GL_LINK_STATUS,&success);
    SM_ASSERT(success,"Failed to link programm");

    //Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    SM_TRACE("Shader ready");
}
void Shader::use(){ 
    glUseProgram(programID);
}
void Shader::setBool(const std::string &name, bool value) const{         
    glUniform1i(glGetUniformLocation(programID,name.c_str()),value); 
}void Shader::setInt(const std::string &name, int value) const{ 
    glUniform1i(glGetUniformLocation(programID,name.c_str()),value); 
}void Shader::setFloat(const std::string &name, float value) const{ 
    glUniform1f(glGetUniformLocation(programID,name.c_str()),value); 
}

Texture::Texture(char* texturePath,BumpAllocator* bumpAllocator,unsigned int internalFormat){
    SM_TRACE("Loading texture");
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //Load texture
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* data=stbi_load(texturePath,&width,&height,&nrChannels,0);
    SM_ASSERT(data,"Failed to load texture");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    SM_TRACE("Texture successfully loaded");
}
void Texture::use(){
    glBindTexture(GL_TEXTURE_2D,textureID);
}