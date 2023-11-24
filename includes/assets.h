#pragma once
#include "LTR_Engine_lib.h"
#include <sstream>
#include <iostream>



#include "render_interface.h"

// ############################################################################
//                            Assets Classes
// ############################################################################

class Transform2D{
public:
    Transform2D(RenderInterface::Texture* texture,glm::vec2 pos,float rot,glm::vec2 size);
    void transform_2D_translate(bool reset,glm::vec2 pos);
    void transform_2D_rotate(bool reset,float rot);
    bool visible;
    glm::mat4 transform_2D_modelMatrix();
    bool addChild(Transform2D* child);
    bool removeChild(Transform2D* child);
private:
    RenderInterface::Texture* texture;
    glm::mat4 pos;
    glm::mat4 rot;
    glm::vec2 size;
    Transform2D* children;
};

