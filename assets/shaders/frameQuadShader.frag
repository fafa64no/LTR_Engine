#version 430 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;
uniform vec2 screenRatio;

void main(){ 
    FragColor=texture(screenTexture,vec2(TexCoords.x*screenRatio.x,TexCoords.y*screenRatio.y));
}