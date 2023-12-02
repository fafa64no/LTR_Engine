#version 430 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;
uniform int pixelation;

void main(){ 
    FragColor=texture(screenTexture,TexCoords/pixelation);
}