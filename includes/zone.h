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

// ############################################################################
//                            Zone Functions
// ############################################################################
Zone::Zone(glm::vec3 ambientLight){
    this->ambientLight=ambientLight;
}

void init_zones_memory(BumpAllocator* persistentStorage){
    testZone=(Zone*)bump_alloc(persistentStorage,sizeof(Zone));
    SM_ASSERT(testZone,"Failed to allocate testZone");
}
void init_zones(){
    testZone=new Zone(
        /*glm::vec3(1.0,1.0,1.0)*/glm::vec3(0.1,0.1,0.1)
    );
}

