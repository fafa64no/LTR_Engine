#version 430 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D textureUsed;

void main(){
    FragColor=texture(textureUsed,TexCoord)*vec4(vertexColor,1.0);
}