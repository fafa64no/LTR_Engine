#version 430 core
out vec4 FragColor;

in VERTEX_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexPos;
    vec3 dirLightDir;
    vec4 FragPosLightSpace;
}fs_int;

in LIGHT_OUT{
    vec3 dirLightColor;
    vec3 ambientLight;
}l_in;

uniform sampler2D textureUsed;
uniform sampler2D shadowMap;

float shadow(){
    vec3 projCoords=0.5*fs_int.FragPosLightSpace.xyz/fs_int.FragPosLightSpace.w+0.5;
    float bias=0.0005;
    return ((projCoords.z-bias>texture(shadowMap,projCoords.xy).r)?1.0:0.0);
}

void main(){
    float direcDiffCoeff=min(max(dot(fs_int.Normal,fs_int.dirLightDir),0.0),1.0)*(1-shadow());
    vec3 lightInfluenceVec=direcDiffCoeff*abs(l_in.dirLightColor)+l_in.ambientLight;
    FragColor=vec4(lightInfluenceVec,1.0)*texture(textureUsed,fs_int.TexPos);
}