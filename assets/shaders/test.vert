#version 430 core
layout (location=0) in vec3 vPos;
layout (loaction=1) in vec3 vColor;
layout (location=2) in vec2 vTexCoord;
  
out vec4 vertexColor;
out vec2 texCoord;

void main(){
    gl_Position=vec4(vPos, 1.0);
    vertexColor=vec4(vColor, 1.0);
    texCoord=vTexCoord;
}