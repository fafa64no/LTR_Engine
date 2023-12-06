#pragma once

#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "render_interface.h"

namespace Scenes{
    // ############################################################################
    //                            Scene declaration
    // ############################################################################
    static RenderInterface::Scene* testShapes;
    static RenderInterface::Scene* fishes;
    static RenderInterface::Scene* baseDecor;

    enum mesh_enum{
        MESHID_CUBE,
        MESHID_ICOSPHERE,
        MESHID_SPACESHIP,
        MESHID_BLOPPY,
        MESHID_PWASCEONAIN,
        MESHID_GROUND,
        MESHID_BUILDINGS,
        MESHID_ROAD,
        MESHID_ROADDECALS,
        MESHID_GRASS,
        MESHID_COUNT
    };
    static RenderInterface::Mesh** meshList;

    // ############################################################################
    //                            Scene build
    // ############################################################################
    int SetupScenes(BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
        //Import scenes
        testShapes=new RenderInterface::Scene("assets/meshes/BasicShapes/testShape.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);
        fishes=new RenderInterface::Scene("assets/meshes/Creatures/Poisson2.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);
        baseDecor=new RenderInterface::Scene("assets/meshes/Decor/baseDecor.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);

        //Alloc memory
        meshList=(RenderInterface::Mesh**)bump_alloc(persistentStorage,sizeof(RenderInterface::Mesh*)*((int)MESHID_COUNT));

        //Store meshes
        SM_TRACE("Storing meshes");
        meshList[MESHID_CUBE]       =testShapes->nodes[testShapes->getNodeWithName("Cube")]->mesh;
        meshList[MESHID_ICOSPHERE]  =testShapes->nodes[testShapes->getNodeWithName("Icosphere")]->mesh;
        meshList[MESHID_SPACESHIP]  =testShapes->nodes[testShapes->getNodeWithName("Spaceship")]->mesh;
        meshList[MESHID_BLOPPY]     =testShapes->nodes[testShapes->getNodeWithName("Bloppy")]->mesh;

        meshList[MESHID_PWASCEONAIN]=fishes->nodes[fishes->getNodeWithName("Cone 3")]->mesh;

        meshList[MESHID_GROUND]     =baseDecor->nodes[baseDecor->getNodeWithName("ground")]->mesh;
        meshList[MESHID_BUILDINGS]  =baseDecor->nodes[baseDecor->getNodeWithName("build1")]->mesh;
        meshList[MESHID_ROAD]       =baseDecor->nodes[baseDecor->getNodeWithName("road")]->mesh;
        meshList[MESHID_ROADDECALS] =baseDecor->nodes[baseDecor->getNodeWithName("roadDecals")]->mesh;
        meshList[MESHID_GRASS]      =baseDecor->nodes[baseDecor->getNodeWithName("grass")]->mesh;

        return 1;
    }
}