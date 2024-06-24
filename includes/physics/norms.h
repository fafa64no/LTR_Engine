#pragma once

#include "LTR_Engine_lib.h"

// ############################################################################
//                            Useful math stuff
// ############################################################################
float max3f(float x,float y,float z){
    return (x>y)?(x>z)?x:z:(y>z)?y:z;
}
float max2f(float x,float y){
    return (x>y)?x:y;
}
float min2f(float x,float y){
    return (x<y)?x:y;
}

// ############################################################################
//                            Norms
// ############################################################################
/**
 * A bunch of norms referring to different shapes
 * @param vec position of object from the collider's view
 * @param size scale of the object around each axis
 * @return <1 if in object, >1 if outside, gives a general idea of distance
*/
float norm_block(glm::vec3 vec,glm::vec3 size){
    return max3f(abs(vec.x)*size.x,abs(vec.y)*size.y,abs(vec.z)*size.z);
}
float norm_sphere(glm::vec3 vec,glm::vec3 size){
    return sqrt(vec.x*vec.x*size.x+vec.y*vec.y*size.y+vec.z*vec.z*size.z);
}
float norm_cylinderZ(glm::vec3 vec,glm::vec3 size){
    return max2f(sqrt(vec.x*vec.x*size.x+vec.y*vec.y*size.y),abs(vec.z)*size.z);
}
float norm_capsuleZ(glm::vec3 vec,glm::vec3 size){
    float r=sqrt(vec.x*vec.x*size.x+vec.y*vec.y*size.y);
    return min2f(max2f(abs(r),abs(vec.z)/size.z),sqrt((abs(vec.z)-size.z)*(abs(vec.z)-size.z)+r*r));
}