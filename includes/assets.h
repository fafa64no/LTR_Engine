#pragma once
#include "LTR_Engine_lib.h"
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ############################################################################
//                            Assets Constants
// ############################################################################
const char* TEXTURE_PATH_LTR="assets/textures/LTR.png";
const char* TEXTURE_PATH_FARID="assets/textures/farid.png";

// ############################################################################
//                            Assets Classes
// ############################################################################
class Shader{
public:
    //The program ID
    unsigned int programID;
    //Constructor
    Shader(char* vertexPath,char* fragmentPath,BumpAllocator* bumpAllocator);
    //Utility
    void use();
    void setBool(const std::string &name,bool value) const;
    void setInt(const std::string &name,int value) const;
    void setFloat(const std::string &name,float value) const;
    void setMat4(const std::string &name,glm::mat4 value) const;
    void setVec3(const std::string &name,glm::vec3 value) const;
};
class Texture{
public:
    //The texture ID
    unsigned int textureID;
    //Constructor
    Texture(char* texturePath,BumpAllocator* bumpAllocator,unsigned int internalFormat);
    //Utility
    void use();
    int width,height,nrChannels;
};
class Transform2D{
public:
    Transform2D(Texture* texture,glm::vec2 pos,float rot,glm::vec2 size);
    void transform_2D_translate(bool reset,glm::vec2 pos);
    void transform_2D_rotate(bool reset,float rot);
    bool visible;
    glm::mat4 transform_2D_modelMatrix();
    bool addChild(Transform2D* child);
    bool removeChild(Transform2D* child);
private:
    Texture* texture;
    glm::mat4 pos;
    glm::mat4 rot;
    glm::vec2 size;
    Transform2D* children;
};

