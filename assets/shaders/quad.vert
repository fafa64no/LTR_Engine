#version 430 core

layout (location=0) out vec2 textureCoordsOut;

void main(){
    //OpenGL coordinates normalized
    // -1/ 1                        1/ 1
    //
    //                 0
    //
    // -1/-1                        1/-1
    vec2 vertices[6]={
        vec2(-1,1),     //Top Left
        vec2(-1,-1),    //Bottom Right
        vec2(1,1),      //Top Right
        vec2(1,1),      //Top Right
        vec2(-1,-1),    //Bottom Left
        vec2(1,-1)      //Bottom Right
    };
    float left=0.0;
    float top=0.0;
    float right=309.0;
    float bottom=309.0;
    vec2 textureCoords[6]={
        vec2(left,top),
        vec2(left,bottom),
        vec2(right,top),
        vec2(right,top),
        vec2(left,bottom),
        vec2(right,bottom)
    };
    gl_Position=vec4(vertices[gl_VertexID],1.0,1.0);
    textureCoordsOut=textureCoords[gl_VertexID];
}