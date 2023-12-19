#version 430 core
layout (location=0) in vec3 vPos;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vTexCoord;
layout (std140) uniform MatsAndVecs{
    mat4 projViewMat;
    mat4 viewMat;
    mat4 dirLightSpaceMat;
    vec3 dirLightDir;
    vec3 dirLightColor;
    vec3 ambientLight;
};

out VERTEX_OUT{
    vec3 Normal;
    vec3 FragPos;
    vec2 TexPos;
    vec3 dirLightDir;
    vec4 FragPosLightSpace;
}vs_out;

out LIGHT_OUT{
    vec3 dirLightColor;
    vec3 ambientLight;
}l_out;

uniform mat4 modelMat;
uniform mat4 normalMat;

void main(){
    gl_Position=projViewMat*modelMat*vec4(vPos,1.0);
    vs_out.Normal=mat3(normalMat)*vNormal;
    vs_out.FragPos=vec3(viewMat*modelMat*vec4(vPos,1.0));
    vs_out.TexPos=vTexCoord;
    vs_out.dirLightDir=mat3(normalMat)*dirLightDir;
    vs_out.FragPosLightSpace=dirLightSpaceMat*modelMat*vec4(vPos,1.0);
    l_out.dirLightColor=dirLightColor;
    l_out.ambientLight=ambientLight;
}