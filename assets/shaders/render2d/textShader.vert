#version 430 core
layout (location=0) in vec2 aPos;
layout (location=1) in vec2 aTexCoords;
out vec2 TexCoords;

uniform mat4 model;
uniform float containerDepth;

void main(){
    gl_Position=model*vec4(aPos,containerDepth,1.0);
    TexCoords=aTexCoords;
}