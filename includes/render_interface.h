#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "LTR_Engine_lib.h"
#include "assets.h"

// ############################################################################
//                            Render Constants
// ############################################################################
constexpr int MAX_2D_TRANSFORMS=1000;
constexpr int MAX_3D_TRANSFORMS=1000;

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
        Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices,unsigned int type);
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        void Draw(Shader &shader);
    private:
        unsigned int VAO,VBO,EBO;
        void setupMesh();
        unsigned int type;
    };
    class Node{
    public:
        Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale);
        Mesh* mesh;
        glm::vec3 position;
        glm::mat4 rotation;
        glm::vec3 scale;
        void Draw(Shader &shader);
    };
    class Scene{
    public:
        Scene(char* scenePath,unsigned int type,BumpAllocator* persistantStorage,BumpAllocator* transientStorage);
        Node** nodes;
        const int getNodeCount();
        void Draw(Shader &shader);
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
        int transformCount;
        Camera* currentCamera;
        Transform2D transforms_2D[MAX_2D_TRANSFORMS];
    };

    static RenderData* renderData;
    static Scene* testScene;
};