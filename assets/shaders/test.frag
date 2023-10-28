#version 430 core
in vec4 vertexColor;
int vec2 texCoord;

out vec4 FragColor;

uniform sampler2D textureUsed;

void main(){
    FragColor=texture(textureUsed,texCoord)*vertexColor;
}