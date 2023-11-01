#version 430 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D textureUsed1;
uniform sampler2D textureUsed2;

uniform vec3 ambientLight;
uniform vec3 lightPos;
uniform vec3 diffuseLightColor;
uniform float diffuseLightStrength;
uniform float diffuseLightRange;

void main(){
    vec3 norm=normalize(Normal);
    vec3 lightDir=lightPos-FragPos;
    float diffCoeff=max(dot(norm,normalize(lightDir)),0.0);
    vec3 diffuseResult=
        diffCoeff
        *diffuseLightStrength
        *diffuseLightColor
        *min(1.0,diffuseLightRange/length(lightDir));

    vec4 textureColor=
        mix(texture(textureUsed1,TexCoord),texture(textureUsed2,TexCoord),0.4)
        *vec4(vertexColor,1.0)
        *vec4(ambientLight+diffuseResult,1.0);
    FragColor=textureColor;
}