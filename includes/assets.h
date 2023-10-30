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
//                            Assets Structs
// ############################################################################
enum SpriteID{
    SPRITE_LTR,
    SPRITE_FARID,
    SPRITE_COUNT
};
struct Sprite{
    glm::ivec2 atlasOffset;
    glm::ivec2 spriteSize;
    const char* spritePath;
};

// ############################################################################
//                            Assets Functions
// ############################################################################
Sprite get_sprite(SpriteID spriteID){
    Sprite sprite={};
    switch(spriteID){
        case SPRITE_LTR:{
            sprite.atlasOffset={0,0};
            sprite.spriteSize={309,309};
            sprite.spritePath=TEXTURE_PATH_LTR;
            break;
        }
        case SPRITE_FARID:{
            sprite.atlasOffset={0,0};
            sprite.spriteSize={1920,1080};
            sprite.spritePath=TEXTURE_PATH_FARID;
            break;
        }
    }
    return sprite;
}

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


