#include "assets.h"
#include "gl_renderer.h"

// ############################################################################
//                            Transform2D Functions
// ############################################################################
Transform2D::Transform2D(RenderInterface::Texture* texture,glm::vec2 pos,float rot,glm::vec2 size){
    this->texture=texture;
    this->pos=glm::translate(glm::mat4(1.0f),glm::vec3(pos.x,pos.y,0.0f));
    this->rot=glm::rotate(glm::mat4(1.0f),rot,glm::vec3(0.0f,0.0f,1.0f));
    this->size=size;
}
void Transform2D::transform_2D_translate(bool reset,glm::vec2 pos){
    if(reset){
        this->pos=glm::translate(glm::mat4(1.0f),glm::vec3(pos.x,pos.y,0.0f));
    }else{
        this->pos=glm::translate(this->pos,glm::vec3(pos.x,pos.y,0.0f));
    }
}
void Transform2D::transform_2D_rotate(bool reset,float rot){
    if(reset){
        this->rot=glm::rotate(glm::mat4(1.0f),rot,glm::vec3(0.0f,0.0f,1.0f));
    }else{
        this->rot=glm::rotate(this->rot,rot,glm::vec3(0.0f,0.0f,1.0f));
    }
}
glm::mat4 Transform2D::transform_2D_modelMatrix(){
    return glm::scale(this->pos*this->rot,glm::vec3(this->size.x,this->size.y,1.0f));
}
bool Transform2D::addChild(Transform2D* child){
    return false;
}
bool Transform2D::removeChild(Transform2D* child){
    return false;
}







