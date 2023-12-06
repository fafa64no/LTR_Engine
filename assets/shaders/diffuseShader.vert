#version 430 core
layout (location=0) in vec3 vPos;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vTexCoord;

out vec3 vertexColor;
out vec3 Normal;
out vec3 FragPos;
out vec3 dirLightDir;
out VERTEX_SHADOW_OUT{
    vec4 FragPosLightSpace;
}vs_out;

uniform mat4 view;
uniform mat4 model;
uniform mat4 proj;
uniform mat4 normalMat;
uniform mat4 dirLightSpaceMatrix;

uniform vec3 meshColor;
uniform vec3 directionalLightDir;

void main(){
    gl_Position=proj*view*model*vec4(vPos,1.0);
    vertexColor=meshColor;
    Normal=mat3(normalMat)*vNormal;
    FragPos=vec3(view*model*vec4(vPos,1.0));
    dirLightDir=mat3(normalMat)*directionalLightDir;
    vs_out.FragPosLightSpace=dirLightSpaceMatrix*model*vec4(vPos,1.0);
}