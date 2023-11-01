#version 430 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
}; 

out vec4 FragColor;

in vec3 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

uniform sampler2D textureUsed1;
uniform sampler2D textureUsed2;

uniform vec3 ambientLight;
uniform vec3 diffuseLightColor;
uniform float diffuseLightStrength;
uniform float diffuseLightRange;

uniform Material material;

void main(){
    vec3 norm=normalize(Normal);
    vec3 lightDir=FragPos-LightPos;

    float diffCoeff=max(dot(norm,-normalize(lightDir)),0.0);
    vec3 diffuseResult=
        diffCoeff
        *diffuseLightStrength
        *diffuseLightColor
        *min(1.0,diffuseLightRange/length(lightDir));
    
    vec3 viewDir=normalize(-FragPos);
    vec3 reflectDir=reflect(normalize(lightDir),norm);
    float specCoeff=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specularResult=
        specCoeff
        *diffuseLightColor;

    vec4 textureColor=
        mix(texture(textureUsed1,TexCoord),texture(textureUsed2,TexCoord),0.4)
        *vec4(vertexColor,1.0)
        *vec4(
            ambientLight*material.ambient
            +diffuseResult*material.diffuse
            +specularResult*material.shininess
            ,1.0);
    FragColor=textureColor;
}