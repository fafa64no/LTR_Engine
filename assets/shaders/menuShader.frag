out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D atlasTexture;
uniform vec2 atlasPos;
uniform vec2 textureSize;
void main(){ 
    FragColor=texture(atlasTexture,vec2(atlasPos.x+TexCoords.x*textureSize.x,atlasPos.y+TexCoords.y*textureSize.y));
}