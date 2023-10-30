#version 430 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D textureUsed1;
uniform sampler2D textureUsed2;

void main(){
    vec4 textureColor=mix(texture(textureUsed1,TexCoord),texture(textureUsed2,TexCoord),0.4)*vec4(vertexColor,1.0);
    if (textureColor.a==0.0){
        discard;
    }
    FragColor=textureColor;
}