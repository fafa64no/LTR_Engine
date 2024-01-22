#version 430 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D textexture;
uniform vec3 textColor;

void main(){    
    color=vec4(textColor,texture(textexture,TexCoords).r);
}
