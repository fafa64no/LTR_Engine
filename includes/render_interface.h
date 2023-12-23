#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "LTR_Engine_lib.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ############################################################################
//                            Render Constants
// ############################################################################
constexpr int MAX_2DNODES_TO_RENDER=512;
constexpr int MAX_2DNODES=4096;
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
    struct Vertex_PN{
        glm::vec3 position;
        glm::vec3 normal;
    };
    struct Vertex_PNT{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 texCoords0;
    };
    struct Vertex_PNC{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color0;
    };
    struct Vertex_PNCT{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color0;
        glm::vec2 texCoords0;
    };
    struct Vertex_PNCTJW{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color0;
        glm::vec3 texCoords0;
        glm::vec3 joints0;
        glm::vec3 weights0;
    };

    // ############################################################################
    //                            OpenGL Stuff
    // ############################################################################
    class Shader{
    public:
        unsigned int programID;
        Shader(char* vertexPath,char* fragmentPath,BumpAllocator* bumpAllocator);
        void use();
        void setBool(const std::string &name,bool value) const;
        void setInt(const std::string &name,int value) const;
        void setFloat(const std::string &name,float value) const;
        void setMat4(const std::string &name,glm::mat4 value) const;
        void setVec3(const std::string &name,glm::vec3 value) const;
        void setVec2(const std::string &name,glm::vec2 value) const;
    };
    class Texture{
    public:
        Texture(char* texturePath,unsigned int internalFormat);
        Texture(char* texturePath,glm::vec4 selection,unsigned int internalFormat);
        void use();
    private:
        unsigned int textureID;
        int width,height,nrChannels;
    };

    // ############################################################################
    //                            2D Stuff
    // ############################################################################
    class Node2D{
    public:
        Node2D();
    };
    class Atlas{
    public:
        Atlas(char* atlasPath,glm::ivec2 textureSize,glm::ivec2 atlasSize,BumpAllocator* bumpAllocator,unsigned int internalFormat);
        void Draw(unsigned int textureId);
    private:
        glm::ivec2 atlasSize,textureSize;
        Texture* atlasTextures;
    };
    
    // ############################################################################
    //                            3D Stuff
    // ############################################################################
    class Mesh{
    public:
        Mesh(MeshConstructor* meshConstructor,char* buffer,unsigned int type);
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        void Draw();
        void CastShadow();
        void DebugTrace();
    private:
        unsigned int VAO,VBO,EBO;
        void setupMesh();
        unsigned int type;
    };
    class Node{
    public:
        Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale);
        Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale,Mesh* mesh,Texture* texture,Shader* shader);
        Node(glm::vec3 position,glm::vec4 rotation,glm::vec3 scale,Mesh* mesh,Texture* texture,Shader* shader,glm::vec3 color);
        Mesh* mesh;
        Texture* texture;
        Shader* shader;
        void Draw(void* renderData);
        void CastShadow(Shader &shader,void* renderData);
        glm::vec3 getPos();
        char name[16]={0};
        void translate(glm::vec3 translation);
        void rotate(glm::vec4 rotation);
        void reScale(glm::vec3 scale);
        Node** childNode;
        unsigned int childrenCount=0;
    private:
        glm::vec3 position,scale,color;
        glm::vec4 rotation;
    };
    class Scene{
    public:
        Scene(char* scenePath,unsigned int type,BumpAllocator* persistantStorage,BumpAllocator* transientStorage);
        Node** nodes;
        const unsigned int getNodeCount();
        void Draw(Shader &shader);
        int unsigned getNodeWithName(char* name);
    private:
        unsigned int nodeCount=0;
    };

    // ############################################################################
    //                            Other Stuff
    // ############################################################################
    enum CameraType{
        PERSPECTIVE_CAMERA,
        CENTERED_PERSPECTIVE_CAMERA,
        ORTHOGRAPHIC_CAMERA,
    };
    class Camera{
    public:
        Camera(glm::vec3 camPos,glm::vec3 camUp,float camNear,float camFar);
        Camera(glm::vec3 camPos,glm::vec3 camUp,glm::vec3 offset,float camNear,float camFar,float camWidth);
        Camera(glm::vec3 camPos,glm::vec3 camUp,float yaw, float pitch,float camNear,float camFar);
        Camera(glm::vec3 camPos,glm::vec3 camUp,float yaw, float pitch,glm::vec3 offset,float camNear,float camFar,float camWidth);

        glm::mat4 viewMat();
        glm::mat4 projMat();
        glm::vec3 frontVec();
        void updateViewMat();
        void updateProjMat();
        void updateCamFront();

        void updateRatio(float aspectRatio);
        void updateRange(float camNear,float camFar);
        void updateZoom(float zoom);
        void setFov(float fov);
        void debugPrint();

        void setPos(glm::vec3 camPos);
        void setDir(float yaw,float pitch);
        void setOffset(glm::vec3 offset);
        void setOffsetYaw(float yaw);
        void updateOffsetHeight(float height);

        void movePos(glm::vec3 camPos);
        void moveRelativePos(glm::vec3 camRelativePos);
        void moveDir(glm::vec2 camTurnDir);
        void moveOffset(glm::vec3 offset);
    private:
        glm::mat4 camView,camProj;
        glm::vec3 camPos,camFront,camUp,offset;
        CameraType cameraType;
        float camNear,camFar,camWidth,aspectRatio{1.0f},yaw{0.0f},pitch{0.0f},fov{45.0f};
    };
    struct RenderData{
        Camera* currentCamera;
        glm::mat4 viewMat,projMat;
        Node* nodes_to_render[MAX_NODES_TO_RENDER];
        unsigned int nodeCount=0;
        unsigned short pixelation;
    };
    struct NodeContainer{
        Node* nodes[MAX_NODES];
        unsigned int nodeCount=0;
    };

    // ############################################################################
    //                            Render Globals
    // ############################################################################
    static RenderData* renderData;
    static NodeContainer* nodeContainer;

    static Camera* freeCam;
    static Camera* playerCam;

    // ############################################################################
    //                            Render Functions
    // ############################################################################
    int addNodeToRender(Node* node){
        if(renderData->nodeCount==MAX_NODES_TO_RENDER)return -1;
        renderData->nodes_to_render[renderData->nodeCount]=node;
        return renderData->nodeCount++;
    }
    int storeNode(Node* node){
        if(nodeContainer->nodeCount==MAX_NODES)return -1;
        nodeContainer->nodes[nodeContainer->nodeCount]=node;
        return nodeContainer->nodeCount++;
    }
};
