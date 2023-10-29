#pragma once
#include "LTR_Engine_lib.h"
#include "assets.h"

// ############################################################################
//                            Render Constants
// ############################################################################
constexpr int MAX_TRANSFORMS=1000;

// ############################################################################
//                            Render Structs
// ############################################################################
struct Transform{
    IVec2 atlasOffset;
    IVec2 spriteSize;
    Vec2 pos;
    Vec2 size;
};
struct RenderData{
    int transformCount;
    Transform transforms[MAX_TRANSFORMS];
};

// ############################################################################
//                            Render Structs
// ############################################################################
static RenderData* renderData;

// ############################################################################
//                            Render Functions
// ############################################################################
void draw_sprite(SpriteID spriteID,Vec2 pos,Vec2 size){
    Sprite sprite=get_sprite(spriteID);
    Transform transform={};
    transform.pos=pos;
    transform.size=size;
    transform.atlasOffset=sprite.atlasOffset;
    transform.spriteSize=sprite.spriteSize;
    renderData->transforms[renderData->transformCount++]=transform;
}

