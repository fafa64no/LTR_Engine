#version 430 core
layout (location=0) in vec3 vPos;
layout (location=1) in vec3 vColor;
layout (location=2) in vec2 vTexCoord;
layout (location=3) in vec3 vNormal;
  
out vec3 vertexColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 normalMat;
uniform vec3 lightPos;

void main(){
    gl_Position=proj*view*model*vec4(vPos,1.0);
    vertexColor=vColor;
    TexCoord=vTexCoord;
    Normal=(normalMat*vec4(vNormal,1.0)).xyz;
    FragPos=(view*model*vec4(vPos,1.0)).xyz;
    LightPos=vec3(view*vec4(lightPos,1.0));
}