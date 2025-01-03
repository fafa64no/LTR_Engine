#pragma once

#include "LTR_Engine_lib.h"

// ############################################################################
//                            Material Structs
// ############################################################################
struct Material{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    int shininess;
}; 

// ############################################################################
//                            Material Globals
// ############################################################################
static Material metal;

// ############################################################################
//                            Material Functions
// ############################################################################
void gl_materials_init(){
    metal.ambient=glm::vec3(0.2,0.2,0.2);
    metal.diffuse=glm::vec3(0.8,0.8,0.8);
    metal.specular=glm::vec3(0.05,0.05,0.05);
    metal.shininess=32;
}