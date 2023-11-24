#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "render_interface.h"

namespace RenderInterface{
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
    void Camera::updateDir(glm::vec2 camTurnDir){
        static float yaw=0.0;
        static float pitch=0.0;
        yaw+=camTurnDir.x*this->sensitivity;
        pitch-=camTurnDir.y*this->sensitivity;
        //Check -90<pitch<90
        if(pitch<-89.0)pitch=-89.0;
        if(pitch>89.0)pitch=89.0;
        //Update cam view
        this->camFront=glm::normalize(glm::vec3(
            cos(glm::radians(yaw))*cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw))*cos(glm::radians(pitch))
        ));
        this->camView=glm::lookAt(this->camPos,this->camPos+this->camFront,this->camUp);
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
        char* pPositions        =&buffer[meshConstructor->positions->bufferView->byteOffset];
        char* pNormals          =&buffer[meshConstructor->normal->bufferView->byteOffset];
        char* pTexturePositions =&buffer[meshConstructor->texcoord->bufferView->byteOffset];
        char* pIndices          =&buffer[meshConstructor->indices->bufferView->byteOffset];
        //Fill vectors
        SM_TRACE("\tFilling vectors");
        for(int i=0;i<meshConstructor->positions->count;i++){
            Vertex vertex{
                glm::vec3(
                    get_float_be(&pPositions[sizeof(float)*(3*i)]),
                    get_float_be(&pPositions[sizeof(float)*(3*i+1)]),
                    get_float_be(&pPositions[sizeof(float)*(3*i+2)])
                ),
                glm::vec3(
                    get_float_be(&pNormals[sizeof(float)*(3*i)]),
                    get_float_be(&pNormals[sizeof(float)*(3*i+1)]),
                    get_float_be(&pNormals[sizeof(float)*(3*i+2)])
                ),
                glm::vec2(
                    get_float_be(&pTexturePositions[sizeof(float)*(2*i)]),
                    get_float_be(&pTexturePositions[sizeof(float)*(2*i+1)])
                ),
                glm::vec3(
                    1.0f,
                    1.0f,
                    1.0f
                )
            };
            this->vertices.push_back(vertex);
        }
        SM_TRACE("\tFilling indices");
        for(int i=0;i<meshConstructor->indices->count;i++){
            unsigned int temp=(unsigned int)get_uint_fshort_be(&pIndices[sizeof(unsigned short)*i]);
            this->indices.push_back(temp);
        }
        SM_TRACE("\tSetup OpenGL");
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
    void Mesh::Draw(Shader &shader){
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES,sizeof(unsigned int)*this->indices.size(),GL_UNSIGNED_INT,nullptr);
    }

    // ############################################################################
    //                            Node Functions
    // ############################################################################
    Node::Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale){
        //SM_TRACE("\tBuilding node");
        this->position=rotation;
        quatToMat(this->rotation,rotation);
        this->scale=(scale.x==0)?glm::vec3(1.0f,1.0f,1.0f):scale;
        this->texture=faridTexture;
        this->shader=testShader;
    }
    Node::Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale,Mesh* mesh,Texture* texture,Shader* shader){
        //SM_TRACE("\tBuilding node");
        this->mesh=mesh;
        this->position=rotation;
        quatToMat(this->rotation,rotation);
        this->scale=(scale.x==0)?glm::vec3(1.0f,1.0f,1.0f):scale;
        this->texture=texture;
        this->shader=shader;
    }
    void Node::Draw(void* renderData){
        glActiveTexture(GL_TEXTURE0);
        this->texture->use();
        this->shader->use();
        ((RenderData*)renderData)->currentCamera->updateDir(input->mouseDir);
        glm::mat4 viewMat=((RenderData*)renderData)->currentCamera->viewMat();
        glm::mat4 projMat=glm::perspective(glm::radians(45.0f), (float)input->screenSize.x/(float)input->screenSize.y, 0.1f, 100.0f);
        glm::mat4 modelMat=glm::mat4(1.0f);
        modelMat=glm::translate(modelMat,this->position);
        //modelMat=modelMat*this->rotation;
        modelMat=glm::scale(modelMat,this->scale);
        this->shader->setMat4("model",modelMat);
        glm::mat4 normalMat=glm::transpose(glm::inverse(RenderInterface::renderData->currentCamera->viewMat()*modelMat));
        this->shader->setMat4("normalMat",normalMat);
        this->mesh->Draw(*this->shader);
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
        int bufferCount,    bufferViewCount,            accessorCount,          meshConstructorCount,               meshNodeConstructorCount{0};
        Buffer* buffers;    BufferView* bufferViews;    Accessor* accessors;    MeshConstructor* meshConstructors;  MeshNodeConstructor* meshNodeConstructors;
        get_glb_structure(transientStorage,
                            *buffer,                binStart,
                            buffers,                bufferCount,
                            bufferViews,            bufferViewCount,
                            accessors,              accessorCount,
                            meshConstructors,       meshConstructorCount,
                            meshNodeConstructors,   meshNodeConstructorCount);
        int incrementDepth{1};
        //Build scene
        SM_TRACE("Building scene");
        this->nodeCount=meshNodeConstructorCount;
        this->nodes=(Node**)bump_alloc(persistentStorage,sizeof(Node*)*this->nodeCount);
        for(int i=0;i<meshNodeConstructorCount;i++){
            //Build nodes
            this->nodes[i]=new Node(
                meshNodeConstructors[i].translation,
                meshNodeConstructors[i].rotation,
                meshNodeConstructors[i].scale
            );
            strcpy(this->nodes[i]->name,meshNodeConstructors[i].name);
            //Build meshes
            this->nodes[i]->mesh=new Mesh(&meshConstructors[i],(char*)&buffer[0][binStart],type);
        }
    }
    const int Scene::getNodeCount(){
        return this->nodeCount;
    }
    void Scene::Draw(Shader &shader){
        for(int i=0;i<nodeCount;i++)this->nodes[i]->Draw(renderData);
    }
    int Scene::getNodeWithName(char* name){
        for(int i=0;i<this->nodeCount;i++){
            if(strcmp(name,this->nodes[i]->name)==0)return i;
        }
        return -1;
    }
}

