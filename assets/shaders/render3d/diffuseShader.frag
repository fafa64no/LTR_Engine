#version 430 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexPos;
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
    vec3 lightInfluenceVec=direcDiffCoeff*abs(directionalLightColor)+ambientLight;
    FragColor=vec4(lightInfluenceVec,1.0)*texture(textureUsed,TexPos);
}