#pragma once

#include "gl_renderer.h"

// ############################################################################
//                            Uniform Buffer Structs
// ############################################################################
struct UniformAligner{
    unsigned int size;
    unsigned int offset;
};

// ############################################################################
//                            Uniform Buffer Align
// ############################################################################
static UniformAligner projViewMatPos;
static UniformAligner viewMatPos;
static UniformAligner dirLightSpaceMatPos;
static UniformAligner dirLightDirPos;
static UniformAligner dirLightColorPos;
static UniformAligner ambientLightPos;
static unsigned int uniformBufferSize;

// ############################################################################
//                            Uniform Buffer Functions
// ############################################################################
void gl_uniformBuffer_setup(){
    unsigned int n=sizeof(float);
    //Sizes
    projViewMatPos.size=16*n;
    viewMatPos.size=16*n;
    dirLightSpaceMatPos.size=16*n;
    dirLightDirPos.size=4*n;
    dirLightColorPos.size=4*n;
    ambientLightPos.size=4*n;

    //Offsets
    projViewMatPos.offset=0;
    viewMatPos.offset=16*n;
    dirLightSpaceMatPos.offset=32*n;
    dirLightDirPos.offset=48*n;
    dirLightColorPos.offset=52*n;
    ambientLightPos.offset=56*n;

    //Buffer size
    uniformBufferSize=60*n;
}

void gl_uniformBuffer_update(glm::mat4 projViewMat,glm::mat4 viewMat,glm::mat4 dirLightSpaceMat,glm::vec3 dirLightDir,glm::vec3 dirLightColor,glm::vec3 ambientLight){
    glBufferSubData(GL_UNIFORM_BUFFER,
        projViewMatPos.offset,
        projViewMatPos.size,
        glm::value_ptr(projViewMat)
    );
    glBufferSubData(GL_UNIFORM_BUFFER,
        viewMatPos.offset,
        viewMatPos.size,
        glm::value_ptr(viewMat)
    );
    glBufferSubData(GL_UNIFORM_BUFFER,
        dirLightSpaceMatPos.offset,
        dirLightSpaceMatPos.size,
        glm::value_ptr(dirLightSpaceMat)
    );
    glm::vec4 temp=glm::vec4(0,dirLightDir);
    glBufferSubData(GL_UNIFORM_BUFFER,
        dirLightDirPos.offset,
        dirLightDirPos.size,
        &temp
    );
    temp=glm::vec4(0,dirLightColor);
    glBufferSubData(GL_UNIFORM_BUFFER,
        dirLightColorPos.offset,
        dirLightColorPos.size,
        &temp
    );
    temp=glm::vec4(0,ambientLight);
    glBufferSubData(GL_UNIFORM_BUFFER,
        ambientLightPos.offset,
        ambientLightPos.size,
        &temp
    );
}

void gldb_uniformBuffer_update(glm::mat4 projViewMat,glm::mat4 viewMat,glm::mat4 dirLightSpaceMat,glm::vec3 dirLightDir,glm::vec3 dirLightColor,glm::vec3 ambientLight){
    glBufferSubData(GL_UNIFORM_BUFFER,
        projViewMatPos.offset,
        projViewMatPos.size,
        glm::value_ptr(projViewMat)
    );
    glBufferSubData(GL_UNIFORM_BUFFER,
        viewMatPos.offset,
        viewMatPos.size,
        glm::value_ptr(viewMat)
    );
    glBufferSubData(GL_UNIFORM_BUFFER,
        dirLightSpaceMatPos.offset,
        dirLightSpaceMatPos.size,
        glm::value_ptr(dirLightSpaceMat)
    );
    glm::vec4 temp1=glm::vec4(0,dirLightDir);
    glBufferSubData(GL_UNIFORM_BUFFER,
        dirLightDirPos.offset,
        dirLightDirPos.size,
        &temp1
    );
    glm::vec4 temp2=glm::vec4(0,dirLightColor);
    glBufferSubData(GL_UNIFORM_BUFFER,
        dirLightColorPos.offset,
        dirLightColorPos.size,
        &temp2
    );
    glm::vec4 temp3=glm::vec4(0,ambientLight);
    glBufferSubData(GL_UNIFORM_BUFFER,
        ambientLightPos.offset,
        ambientLightPos.size,
        &temp3
    );
    char msg[512]{0};
    sprintf(msg,"===== uniform buffer update =====\nprojViewMat:\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f\n\t%f\t%f\t%f\t%f"
        "\n===== uniform buffer update end =====\n",
        projViewMat[0][0],
        projViewMat[1][0],
        projViewMat[2][0],
        projViewMat[3][0],
        projViewMat[0][1],
        projViewMat[1][1],
        projViewMat[2][1],
        projViewMat[3][1],
        projViewMat[0][2],
        projViewMat[1][2],
        projViewMat[2][2],
        projViewMat[3][2],
        projViewMat[0][3],
        projViewMat[1][3],
        projViewMat[2][3],
        projViewMat[3][3]
    );
    SM_TRACE(msg);
}