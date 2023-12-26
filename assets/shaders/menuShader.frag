#version 430 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D usedTexture;
void main(){ 
    FragColor=texture(usedTexture,TexCoords);
}