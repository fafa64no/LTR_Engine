#pragma once

#include "LTR_Engine_lib.h"

// ############################################################################
//                            Zone Structs
// ############################################################################
class Zone{
public:
    Zone(glm::vec3 ambientLight);
    glm::vec3 ambientLight;
};

// ############################################################################
//                            Zone Globals
// ############################################################################
static Zone* testZone;
static Zone* fullBrightZone;

// ############################################################################
//                            Zone Functions
// ############################################################################
Zone::Zone(glm::vec3 ambientLight){
    this->ambientLight=ambientLight;
}

void init_zones(){
    testZone=new Zone(
        glm::vec3(0.1f,0.1f,0.3f)
    );
    fullBrightZone=new Zone(
        glm::vec3(2.0f,2.0f,2.0f)
    );
}

