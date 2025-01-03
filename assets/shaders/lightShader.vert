#version 430 core
layout (location=0) in vec3 vPos;
layout (location=1) in vec3 vColor;
  
out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
    gl_Position=proj*view*model*vec4(vPos,1.0);
    vertexColor=vColor;
}