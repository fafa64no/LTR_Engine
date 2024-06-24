#pragma once

#include "LTR_Engine_lib.h"

// ############################################################################
//                            Classes and structs
// ############################################################################
class Collider{
public:
    Collider(float (*used_norm)(glm::vec3,glm::vec3));
    float distance(glm::vec3 position);
private:
    float (*used_norm)(glm::vec3,glm::vec3);
    glm::vec3 col_pos;
    glm::vec4 col_rot;
    glm::vec3 col_config;
};

// ############################################################################
//                            Collider Storage
// ############################################################################

// ############################################################################
//                            Update and initialisation
// ############################################################################

