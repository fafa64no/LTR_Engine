#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "LTR_Engine_lib.h"
#include "assets.h"

// ############################################################################
//                            Render Constants
// ############################################################################
constexpr int MAX_2D_TRANSFORMS_TO_RENDER=1000;
constexpr int MAX_2D_TRANSFORMS=1000;
constexpr int MAX_NODES_TO_RENDER=512;
constexpr int MAX_NODES=4096;

// ############################################################################
//                            Render Structs and Classes
// ############################################################################
namespace RenderInterface{
    struct Vertex{
        glm::vec3 Position;
        glm::vec3 Color;
        glm::vec2 TexCoords;
        glm::vec3 Normal;
    };
    
    class Mesh{
    public:
        Mesh(MeshConstructor* meshConstructor,char* buffer,unsigned int type);
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        void Draw(Shader &shader);
    private:
        unsigned int VAO,VBO,EBO;
        void setupMesh();
        unsigned int type;
    };
    class Node{
    public:
        Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale);
        Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale,Mesh* mesh,Texture* texture,Shader* shader);
        Mesh* mesh;
        Texture* texture;
        Shader* shader;
        void Draw(void* renderData);
        char name[16]={0};
        void translate(glm::vec3 translation);
        void rotate(glm::vec4 rotation);
        void reScale(glm::vec3 scale);
        Node** childNode;
        int childrenCount=0;
    private:
        glm::vec3 position;
        glm::mat4 rotation;
        glm::vec3 scale;
    };
    class Scene{
    public:
        Scene(char* scenePath,unsigned int type,BumpAllocator* persistantStorage,BumpAllocator* transientStorage);
        Node** nodes;
        const int getNodeCount();
        void Draw(Shader &shader);
        int getNodeWithName(char* name);
    private:
        int nodeCount=0;
    };

    class Camera{
    public:
        Camera(glm::vec3 camPos,glm::vec3 camFront,glm::vec3 camUp);
        glm::mat4 viewMat();
        void updatePos(glm::vec3 camPos);
        void updateDir(glm::vec2 camTurnDir);
        float sensitivity=0.2;
        glm::vec3 camPos;
        glm::vec3 camFront;
        glm::vec3 camUp;
    private:
        glm::mat4 camView;
    };
    struct RenderData{
        Camera* currentCamera;
        Transform2D* transforms_to_render_2D[MAX_2D_TRANSFORMS];
        int transformCount=0;
        Node* nodes_to_render[MAX_NODES_TO_RENDER];
        int nodeCount=0;
    };
    struct NodeContainer{
        Node* nodes[MAX_NODES];
        int nodeCount=0;
    };
    // ############################################################################
    //                            Render Functions
    // ############################################################################
    int addNodeToRender(RenderData* renderData,Node* node){
        if(renderData->nodeCount==MAX_NODES_TO_RENDER)return -1;
        renderData->nodes_to_render[renderData->nodeCount]=node;
        return renderData->nodeCount++;
    }
    int storeNode(NodeContainer* nodeContainer,Node* node){
        SM_TRACE("blurp");
        if(nodeContainer->nodeCount==MAX_NODES)return -1;
        SM_TRACE("blurp");
        nodeContainer->nodes[nodeContainer->nodeCount]=node;
        SM_TRACE("blurp");
        return nodeContainer->nodeCount++;
    }

    // ############################################################################
    //                            Render Globals
    // ############################################################################

    static RenderData* renderData;
    static NodeContainer* nodeContainer;
    static Scene* testScene;
};
