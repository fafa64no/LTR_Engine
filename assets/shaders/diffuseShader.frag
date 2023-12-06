#version 430 core
out vec4 FragColor;

in vec3 vertexColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 dirLightDir;
in VERTEX_SHADOW_OUT{
    vec4 FragPosLightSpace;
}fs_int;

uniform vec3 ambientLight;
uniform vec3 directionalLightColor;

uniform sampler2D textureUsed;
uniform sampler2D shadowMap;

float shadow(){
    vec3 projCoords=0.5*fs_int.FragPosLightSpace.xyz/fs_int.FragPosLightSpace.w+0.5;
    float bias=0.0005;
    return ((projCoords.z-bias>texture(shadowMap,projCoords.xy).r)?1.0:0.0);
}

void main(){
    float direcDiffCoeff=min(max(dot(Normal,dirLightDir),0.0),1.0)*(1-shadow());
    float lightInfluence=direcDiffCoeff+ambientLight.x;
    vec3 lightInfluenceVec=direcDiffCoeff*directionalLightColor+ambientLight;
    vec3 textureColor=lightInfluence*vertexColor;
    FragColor=vec4(textureColor,1.0);
}