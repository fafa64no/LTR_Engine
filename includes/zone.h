#pragma once

#include "LTR_Engine_lib.h"
#include "render_interface.h"
#include "scenes.h"

namespace Zones{
    // ############################################################################
    //                            Zone Structs and classes
    // ############################################################################
    class Biome{
    public:
        Biome(glm::vec3 ambientLight,glm::vec3 sunDir,glm::vec3 sunCol);
        glm::vec3 ambientLight;
        glm::vec3 sunDir;
        glm::vec3 sunCol;
        void updateLightSpaceMatrix();
        glm::mat4 getLightSpaceMatrix();
    private:
        RenderInterface::Camera* camera;
        glm::mat4 lightSpaceMatrix;
    };
    struct RegionMesh{
        int mesh;
        RenderInterface::Texture* texture;
        glm::vec3 position;
    };
    class Region{
    public:
        Region(RegionMesh* meshes,int meshCount,BumpAllocator* bumpAllocator);
        void Draw();
    private:
        int *terrainNodes{nullptr},*propsNodes{nullptr},terrainNodeCount{0},propsNodeCount{0};
        void SetupTerrain(RegionMesh* meshes,int meshCount);
        void SetupProps();
        void DrawTerrain();
        void DrawProps();
    };

    // ############################################################################
    //                            Zone Globals
    // ############################################################################
    static Biome* testBiome;
    static Biome* fullBrightBiome;
    static Region* testRegion;

    // ############################################################################
    //                            Biome Functions
    // ############################################################################
    Biome::Biome(glm::vec3 ambientLight,glm::vec3 sunDir,glm::vec3 sunCol){
        this->ambientLight=ambientLight;
        this->sunDir=sunDir;
        this->sunCol=sunCol;
        this->camera=new RenderInterface::Camera(
            -100.0f*sunDir,
            glm::normalize(sunDir),
            glm::vec3(0.0f,1.0f,0.0f)
        );
    }
    void Biome::updateLightSpaceMatrix(){
        const float range=500.0f;
        glm::mat4 lightProjection=glm::ortho(-range,range,-range,range,1.0f,1000.0f);
        this->lightSpaceMatrix=lightProjection*this->camera->viewMat();
    }
    glm::mat4 Biome::getLightSpaceMatrix(){
        return this->lightSpaceMatrix;
    }

    // ############################################################################
    //                            Region Functions
    // ############################################################################
    Region::Region(RegionMesh* meshes,int meshCount,BumpAllocator* bumpAllocator){
        this->terrainNodeCount=meshCount;
        this->terrainNodes=(int*)bump_alloc(bumpAllocator,meshCount*sizeof(int));
        this->SetupTerrain(meshes,meshCount);
        this->SetupProps();
    }
    void Region::SetupTerrain(RegionMesh* meshes,int meshCount){
        for(int i=0;i<meshCount;i++)this->terrainNodes[i]=RenderInterface::storeNode(RenderInterface::nodeContainer,new RenderInterface::Node(
            meshes[i].position,
            glm::vec4(0),
            glm::vec3(1.0f,1.0f,1.0f),
            Scenes::meshList[meshes[i].mesh],
            meshes[i].texture,
            diffuseShader
        ));
    }
    void Region::SetupProps(){}
    void Region::Draw(){
        this->DrawTerrain();
        this->DrawProps();
    }
    void Region::DrawTerrain(){
        for(int i=0;i<this->terrainNodeCount;i++)RenderInterface::addNodeToRender(RenderInterface::renderData,RenderInterface::nodeContainer->nodes[this->terrainNodes[i]]);
    }
    void Region::DrawProps(){}

    // ############################################################################
    //                            Zone Functions
    // ############################################################################
    void init_zones(BumpAllocator* persistentStorage){
        //Biomes
        SM_TRACE("Biome setup");
        testBiome=new Biome(
            glm::vec3(0.2f,0.05f,0.05f),
            glm::normalize(glm::vec3(-1.0f,-1.0f,-1.0f)),
            glm::vec3(0.7f,0.6f,1.0f)
        );
        fullBrightBiome=new Biome(
            glm::vec3(1.0f,1.0f,1.0f),
            glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(0.0f,0.0f,0.0f)
        );
        //Regions
        SM_TRACE("Region setup");
        const int nmeshes=4;
        int testRegionMeshes[nmeshes]={
            Scenes::MESHID_GROUND,
            Scenes::MESHID_BUILDING1,
            Scenes::MESHID_BUILDING2,
            Scenes::MESHID_BUILDING3
        };
        glm::vec3 testRegionPositions[nmeshes]={
            glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(41.0f,0.0f,41.0f),
            glm::vec3(41.0f,0.0f,-88.0f),
            -glm::vec3(88.0f,0.0f,41.0f)
        };
        RenderInterface::Texture* testRegionTextures[nmeshes]={
            groundTexture,
            building1Texture,
            building2Texture,
            building3Texture
        };
        RegionMesh meshes[nmeshes];
        for(int i=0;i<nmeshes;i++){
            meshes[i].mesh=testRegionMeshes[i];
            meshes[i].position=testRegionPositions[i];
            meshes[i].texture=testRegionTextures[i];
        }
        testRegion=new Region(
            meshes,
            nmeshes,
            persistentStorage
        );
    }
}