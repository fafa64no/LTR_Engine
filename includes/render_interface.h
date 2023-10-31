#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "LTR_Engine_lib.h"
#include "assets.h"

// ############################################################################
//                            Render Constants
// ############################################################################
constexpr int MAX_TRANSFORMS=1000;

// ############################################################################
//                            Render Structs and Classes
// ############################################################################
struct Transform{
    glm::ivec2 atlasOffset;
    glm::ivec2 spriteSize;
    glm::ivec2 pos;
    glm::ivec2 size;
};
class Camera{
public:
    Camera(glm::vec3 camPos,glm::vec3 camFront,glm::vec3 camUp);
    glm::mat4 viewMat();
    void updatePos(glm::vec3 camPos);
    void updateDir(glm::vec2 camTurnDir);
    float sensitivity=0.2;
    glm::vec3 camPos;
    glm::vec3 camFront;
    glm::vec3 camUp;
private:
    glm::mat4 camView;
};
struct RenderData{
    int transformCount;
    Transform transforms[MAX_TRANSFORMS];
    Camera* currentCamera;
};

static RenderData* renderData;

// ############################################################################
//                            Render Utility
// ############################################################################

// ############################################################################
//                            Render Functions
// ############################################################################
void draw_sprite(SpriteID spriteID,glm::vec2 pos,glm::vec2 size){
    Sprite sprite=get_sprite(spriteID);
    Transform transform={};
    transform.pos=pos;
    transform.size=size;
    transform.atlasOffset=sprite.atlasOffset;
    transform.spriteSize=sprite.spriteSize;
    renderData->transforms[renderData->transformCount++]=transform;
}

// ############################################################################
//                            Camera Functions
// ############################################################################
Camera::Camera(glm::vec3 camPos,glm::vec3 camFront,glm::vec3 camUp){
    this->camFront=glm::normalize(camFront);
    this->camPos=camPos;
    this->camUp=camUp;
    this->camView=glm::lookAt(this->camPos,this->camPos+this->camFront,this->camUp);
}
glm::mat4 Camera::viewMat(){
    return this->camView;
}
void Camera::updatePos(glm::vec3 camPos){
    this->camPos=camPos;
    this->camView=glm::lookAt(this->camPos,this->camPos+this->camFront,this->camUp);
}void Camera::updateDir(glm::vec2 camTurnDir){
    static float yaw=0.0;
    static float pitch=0.0;
    yaw+=camTurnDir.x*this->sensitivity;
    pitch-=camTurnDir.y*this->sensitivity;
    //Check -90<pitch<90
    if(pitch<-89.0)pitch=-89.0;
    if(pitch>89.0)pitch=89.0;
    //Update cam view
    this->camFront=glm::normalize(glm::vec3(
        cos(glm::radians(yaw))*cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw))*cos(glm::radians(pitch))
    ));
    this->camView=glm::lookAt(this->camPos,this->camPos+this->camFront,this->camUp);
}