#pragma once

#include "LTR_Engine_lib.h"
#include "render_interface.h"
#include "scenes.h"

namespace Zones{
    // ############################################################################
    //                            Zone Structs
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
    class Region{
    public:
        Region(int* meshes,int meshCount,BumpAllocator* bumpAllocator);
        void Draw();
    private:
        int *terrainNodes{nullptr},*propsNodes{nullptr},terrainNodeCount{0},propsNodeCount{0};
        void SetupTerrain(int* meshes,int meshCount);
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
        const float range=100.0f;
        glm::mat4 lightProjection=glm::ortho(-range,range,-range,range,1.0f,750.0f);
        this->lightSpaceMatrix=lightProjection*this->camera->viewMat();
    }
    glm::mat4 Biome::getLightSpaceMatrix(){
        return this->lightSpaceMatrix;
    }

    // ############################################################################
    //                            Region Functions
    // ############################################################################
    Region::Region(int* meshes,int meshCount,BumpAllocator* bumpAllocator){
        this->terrainNodeCount=meshCount;
        this->terrainNodes=(int*)bump_alloc(bumpAllocator,meshCount*sizeof(int));
        this->SetupTerrain(meshes,meshCount);
        this->SetupProps();
    }
    void Region::SetupTerrain(int* meshes,int meshCount){
        glm::vec3 colors[5]={
            glm::vec3(0.35f),
            glm::vec3(0.2f),
            glm::vec3(0.1f),
            glm::vec3(1.0f),
            glm::vec3(0.1f,0.3f,0.1f)
        };
        for(int i=0;i<meshCount;i++)this->terrainNodes[i]=RenderInterface::storeNode(RenderInterface::nodeContainer,new RenderInterface::Node(
            glm::vec3(0),
            glm::vec4(0),
            glm::vec3(1.0f,1.0f,1.0f),
            Scenes::meshList[meshes[i]],
            faridTexture,
            diffuseShader,
            colors[i]
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
            glm::vec3(0.2f,0.2f,0.2f),
            glm::normalize(glm::vec3(-1.0f,-1.0f,-1.0f)),
            glm::vec3(0.8f,0.8f,0.8f)
        );
        fullBrightBiome=new Biome(
            glm::vec3(2.0f,2.0f,2.0f),
            glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(0.0f,0.0f,0.0f)
        );
        //Regions
        SM_TRACE("Region setup");
        int testRegionMeshes[5]={
            Scenes::MESHID_BUILDINGS,
            Scenes::MESHID_GROUND,
            Scenes::MESHID_ROAD,
            Scenes::MESHID_ROADDECALS,
            Scenes::MESHID_GRASS
        };
        testRegion=new Region(
            testRegionMeshes,
            5,
            persistentStorage
        );
    }
}