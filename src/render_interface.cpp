#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "render_interface.h"
#include "game.h"

namespace RenderInterface{
    // ############################################################################
    //                            Shader Functions
    // ############################################################################
    Shader::Shader(char* vertexPath, char* fragmentPath,BumpAllocator* bumpAllocator){
    // 1. retrieve the vertex/fragment source code from filePath
        int vShaderFileSize,fShaderFileSize;
        const char* vShaderCode=read_file(vertexPath,&vShaderFileSize,bumpAllocator);
        const char* fShaderCode=read_file(fragmentPath,&fShaderFileSize,bumpAllocator);
        SM_ASSERT(vShaderCode,"Failed to read shader: %s",vertexPath);
        SM_ASSERT(fShaderCode,"Failed to read shader: %s",fragmentPath);
    // 2. compile shaders
        unsigned int vertexShader,fragmentShader;
        int success;

        //Vertex Shader
        vertexShader=glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader,1,&vShaderCode,NULL);
        glCompileShader(vertexShader);
        //Check compiling
        glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
        SM_ASSERT(success,"Failed to compile Vertex Shader");

        //Fragment Shader
        fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader,1,&fShaderCode,NULL);
        glCompileShader(fragmentShader);
        //Check compiling
        glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
        SM_ASSERT(success,"Failed to compile Fragment Shader");

        //Shader programm
        programID=glCreateProgram();
        glAttachShader(programID,vertexShader);
        glAttachShader(programID,fragmentShader);
        glLinkProgram(programID);
        //Check linking
        glGetProgramiv(programID,GL_LINK_STATUS,&success);
        SM_ASSERT(success,"Failed to link programm");

        //Clean up
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    void Shader::use(){ 
        glUseProgram(programID);
    }
    void Shader::setBool(const std::string &name,bool value) const{         
        glUniform1i(glGetUniformLocation(programID,name.c_str()),value); 
    }void Shader::setInt(const std::string &name,int value) const{ 
        glUniform1i(glGetUniformLocation(programID,name.c_str()),value); 
    }void Shader::setFloat(const std::string &name,float value) const{ 
        glUniform1f(glGetUniformLocation(programID,name.c_str()),value); 
    }void Shader::setMat4(const std::string &name,glm::mat4 value) const{
        glUniformMatrix4fv(glGetUniformLocation(programID,name.c_str()),1,GL_FALSE,glm::value_ptr(value));
    }void Shader::setVec3(const std::string &name,glm::vec3 value) const{
        glUniform3fv(glGetUniformLocation(programID,name.c_str()),1,glm::value_ptr(value));
    }void Shader::setVec2(const std::string &name,glm::vec2 value) const{
        glUniform2fv(glGetUniformLocation(programID,name.c_str()),1,glm::value_ptr(value));
    }

    // ############################################################################
    //                            Texture Functions
    // ############################################################################
    Texture::Texture(char* texturePath,unsigned int internalFormat){
        glGenTextures(1,&textureID);
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        //Load texture
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data=stbi_load(texturePath,&width,&height,&nrChannels,0);
        SM_ASSERT(data,"Failed to load texture");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }
    Texture::Texture(char* texturePath,glm::vec4 selection,unsigned int internalFormat){
        glGenTextures(1,&textureID);
        glBindTexture(GL_TEXTURE_2D,textureID);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        /// TODO OOOOOO
    }
    void Texture::use(){
        glBindTexture(GL_TEXTURE_2D,textureID);
    }

    // ############################################################################
    //                            Atlas Functions
    // ############################################################################
    Atlas::Atlas(char* atlasPath,glm::ivec2 textureSize,glm::ivec2 atlasSize,BumpAllocator* bumpAllocator,unsigned int internalFormat){
        this->atlasSize=atlasSize;
        this->textureSize=textureSize;
        this->atlasTextures=new Texture(atlasPath,internalFormat);
    }

    // ############################################################################
    //                            Mesh Functions
    // ############################################################################
    Mesh::Mesh(MeshConstructor* meshConstructor,char* buffer,unsigned int type){
        SM_TRACE("\tBuilding mesh");
        this->type=type;
        //Init size of vectors and pointers
        this->vertices.reserve(meshConstructor->positions->count*sizeof(RenderInterface::Vertex));
        this->indices.reserve(meshConstructor->indices->count*sizeof(unsigned int));
        float* pPositions        =(float*)&buffer[meshConstructor->positions->bufferView->byteOffset];
        float* pNormals          =(float*)&buffer[meshConstructor->normal->bufferView->byteOffset];
        float* pTexturePositions =(float*)&buffer[meshConstructor->texcoord->bufferView->byteOffset];
        unsigned short* pIndices =(unsigned short*)&buffer[meshConstructor->indices->bufferView->byteOffset];
        //Fill vectors
        //SM_TRACE("\tFilling vectors");
        for(int i=0;i<meshConstructor->positions->count;i++){
            Vertex vertex{
                glm::vec3(
                    pPositions[3*i],
                    pPositions[3*i+1],
                    pPositions[3*i+2]
                ),
                glm::vec3(
                    pNormals[3*i],
                    pNormals[3*i+1],
                    pNormals[3*i+2]
                ),
                glm::vec2(
                    pTexturePositions[2*i],
                    pTexturePositions[2*i+1]
                ),
                glm::vec3(
                    1.0f,
                    1.0f,
                    1.0f
                )
            };
            this->vertices.push_back(vertex);
        }
        //SM_TRACE("\tFilling indices");
        for(int i=0;i<meshConstructor->indices->count;i++){
            this->indices.push_back((unsigned int)pIndices[i]&0x0000ffff);
        }
        //SM_TRACE("\tSetup OpenGL");
        //Setup OpenGL
        this->setupMesh();
    }
    void Mesh::setupMesh(){
        glGenVertexArrays(1,&(this->VAO));
        glGenBuffers(1,&(this->VBO));
        glGenBuffers(1,&(this->EBO));

        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
        glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),&vertices[0],this->type);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,this->indices.size()*sizeof(unsigned int),&(this->indices[0]),this->type);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(6*sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)(8*sizeof(float)));

        glBindVertexArray(0);
    }
    void Mesh::Draw(){
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES,sizeof(unsigned int)*this->indices.size(),GL_UNSIGNED_INT,nullptr);
    }
    void Mesh::CastShadow(){
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES,sizeof(unsigned int)*this->indices.size(),GL_UNSIGNED_INT,nullptr);
    }

    // ############################################################################
    //                            Node Functions
    // ############################################################################
    Node::Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale){
        //SM_TRACE("\tBuilding node");
        this->position=position;
        quatToMat(this->rotation,rotation);
        this->scale=(scale.x==0)?glm::vec3(1.0f,1.0f,1.0f):scale;
        this->texture=faridTexture;
        this->shader=testShader;
    }
    Node::Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale,Mesh* mesh,Texture* texture,Shader* shader){
        SM_TRACE("\tBuilding node");
        this->mesh=mesh;
        this->position=position;
        quatToMat(this->rotation,rotation);
        this->scale=(scale.x==0)?glm::vec3(1.0f,1.0f,1.0f):scale;
        this->texture=texture;
        this->shader=shader;
        this->color=glm::vec3(1.0f,1.0f,1.0f);
    }
    Node::Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale,Mesh* mesh,Texture* texture,Shader* shader,glm::vec3 color){
        SM_TRACE("\tBuilding node");
        this->mesh=mesh;
        this->position=position;
        quatToMat(this->rotation,rotation);
        this->scale=(scale.x==0)?glm::vec3(1.0f,1.0f,1.0f):scale;
        this->texture=texture;
        this->shader=shader;
        this->color=color;
    }
    void Node::Draw(void* renderData){
        glActiveTexture(GL_TEXTURE0);
        this->texture->use();
        this->shader->use();
        this->shader->setInt("textureUsed",0);
        this->shader->setInt("shadowMap",1);
        glm::mat4 viewMat=((RenderData*)renderData)->currentCamera->viewMat();
        glm::mat4 projMat=glm::perspective(glm::radians(45.0f),(float)input->screenSize.x/(float)input->screenSize.y, 0.1f, 100.0f);
        glm::mat4 modelMat=glm::mat4(1.0f);
        modelMat=glm::translate(modelMat,this->position);
        //modelMat=modelMat*this->rotation;
        modelMat=glm::scale(modelMat,this->scale);
        this->shader->setMat4("model",modelMat);
        this->shader->setMat4("view",((RenderData*)renderData)->viewMat);
        this->shader->setMat4("proj",((RenderData*)renderData)->projMat);
        glm::mat4 normalMat=glm::transpose(glm::inverse(((RenderData*)renderData)->viewMat*modelMat));
        this->shader->setMat4("normalMat",normalMat);
        this->shader->setVec3("meshColor",this->color);
        this->shader->setVec3("ambientLight",gameData->currentBiome->ambientLight);
        this->shader->setVec3("directionalLightColor",gameData->currentBiome->sunDir);
        this->shader->setVec3("directionalLightDir",gameData->currentBiome->sunCol);
        this->shader->setMat4("dirLightSpaceMatrix",gameData->currentBiome->getLightSpaceMatrix());
        this->mesh->Draw();
        for(int i=0;i<this->childrenCount;i++)this->childNode[i]->Draw(renderData);
    }
    void Node::CastShadow(Shader &shader,void* renderData){
        shader.use();
        glm::mat4 modelMat=glm::mat4(1.0f);
        modelMat=glm::translate(modelMat,this->position);
        //modelMat=modelMat*this->rotation;
        modelMat=glm::scale(modelMat,this->scale);
        shader.setMat4("model",modelMat);
        shader.setMat4("lightSpaceMatrix",gameData->currentBiome->getLightSpaceMatrix());
        this->mesh->CastShadow();
        for(int i=0;i<this->childrenCount;i++)this->childNode[i]->CastShadow(shader,renderData);
    }
    void Node::translate(glm::vec3 translation){
        this->position+=translation;
        for(int i=0;i<this->childrenCount;i++)this->childNode[i]->translate(translation);
    }
    void Node::rotate(glm::vec4 rotation){
        //this->rotation=rotation;
        for(int i=0;i<this->childrenCount;i++)this->childNode[i]->rotate(rotation);
    }
    void Node::reScale(glm::vec3 scale){
        this->scale=scale;
        for(int i=0;i<this->childrenCount;i++)this->childNode[i]->reScale(scale);
    }

    // ############################################################################
    //                            Scene Functions
    // ############################################################################
    Scene::Scene(char* scenePath,unsigned int type,BumpAllocator* persistentStorage,BumpAllocator* transientStorage){
        std::vector<char>* buffer;
        read_glb_file(scenePath,buffer,persistentStorage);
        int binStart{-1};
        int bufferCount{0}, bufferViewCount{0},         accessorCount{0},       skinConstructorCount{0},            materialConstructorCount{0},                meshConstructorCount{0},            nodeConstructorCount{0},            animationConstructorCount{0};
        Buffer* buffers;    BufferView* bufferViews;    Accessor* accessors;    SkinConstructor* skinConstructors;  MaterialConstructor* materialConstructors;  MeshConstructor* meshConstructors;  NodeConstructor* nodeConstructors;  AnimationConstructor* animationConstructors;
        get_glb_structure(transientStorage,
                            *buffer,                binStart,
                            buffers,                bufferCount,
                            bufferViews,            bufferViewCount,
                            accessors,              accessorCount,
                            skinConstructors,       skinConstructorCount,
                            materialConstructors,   materialConstructorCount,
                            meshConstructors,       meshConstructorCount,
                            nodeConstructors,       nodeConstructorCount,
                            animationConstructors,  animationConstructorCount);
        int incrementDepth{1};
        //Build scene
        SM_TRACE("Building scene");
        SM_TRACE((char*)std::to_string(nodeConstructorCount).c_str());
        this->nodeCount=nodeConstructorCount;
        this->nodes=(Node**)bump_alloc(persistentStorage,sizeof(Node*)*this->nodeCount);
        for(int i=0;i<nodeConstructorCount;i++){
            //Build nodes
            this->nodes[i]=new Node(
                nodeConstructors[i].translation,
                nodeConstructors[i].rotation,
                nodeConstructors[i].scale
            );
            strcpy(this->nodes[i]->name,nodeConstructors[i].name);
            //Build meshes
            if(nodeConstructors[i].meshConstructor)this->nodes[i]->mesh=new Mesh(nodeConstructors[i].meshConstructor,(char*)&buffer[0][binStart],type);
        }
    }
    const unsigned int Scene::getNodeCount(){
        return this->nodeCount;
    }
    void Scene::Draw(Shader &shader){
        for(int i=0;i<nodeCount;i++)this->nodes[i]->Draw(renderData);
    }
    unsigned int Scene::getNodeWithName(char* name){
        for(int i=0;i<this->nodeCount;i++){
            if(strcmp(name,this->nodes[i]->name)==0)return i;
        }return -1;
    }

    // ############################################################################
    //                            Camera Functions
    // ############################################################################
    Camera::Camera(glm::vec3 camPos,glm::vec3 camFront,glm::vec3 camUp){
        this->camFront=glm::normalize(camFront);
        this->camPos=camPos;
        this->camUp=camUp;
        this->camView=glm::lookAt(this->camPos,this->camPos+this->camFront,this->camUp);
    }
    glm::mat4 RenderInterface::Camera::viewMat(){
        return this->camView;
    }
    void Camera::updatePos(glm::vec3 camPos){
        this->camPos=camPos;
        this->camView=glm::lookAt(this->camPos,this->camPos+this->camFront,this->camUp);
    }
    void Camera::updateDir(glm::vec2 camTurnDir,float dt){
        static float yaw=0.0;
        static float pitch=0.0;
        yaw+=camTurnDir.x*dt*input->sensivity;
        pitch-=camTurnDir.y*dt*input->sensivity;
        //Check -90<pitch<90
        if(pitch<-89.0){pitch=-89.0;}
        else if(pitch>89.0)pitch=89.0;
        //Update cam view
        this->camFront=glm::normalize(glm::vec3(
            cos(glm::radians(yaw))*cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw))*cos(glm::radians(pitch))
        ));
        this->camView=glm::lookAt(this->camPos,this->camPos+this->camFront,this->camUp);
    }
}

