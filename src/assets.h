#pragma once
#include "LTR_Engine_lib.h"

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
    IVec2 atlasOffset;
    IVec2 spriteSize;
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



