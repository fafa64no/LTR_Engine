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
    static RenderInterface::Scene* fishes2;
    static RenderInterface::Scene* baseDecor;
    static RenderInterface::Scene* baseDecor2;

    enum mesh_enum{
        MESHID_CUBE,
        MESHID_ICOSPHERE,
        MESHID_SPACESHIP,
        MESHID_BLOPPY,
        MESHID_PWASCEONAIN,
        MESHID_CHINOSAURE,
        MESHID_GROUND,
        MESHID_BUILDING1,
        MESHID_BUILDING2,
        MESHID_BUILDING3,
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
        //fishes2= new RenderInterface::Scene("assets/meshes/Creatures/Chinosaure_decimated.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);
        baseDecor=new RenderInterface::Scene("assets/meshes/Decor/baseDecor.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);
        baseDecor2=new RenderInterface::Scene("assets/meshes/Decor/baseDecor2.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);

        //Alloc memory
        meshList=(RenderInterface::Mesh**)bump_alloc(persistentStorage,sizeof(RenderInterface::Mesh*)*((int)MESHID_COUNT));

        //Store meshes
        SM_TRACE("Storing meshes");
        meshList[MESHID_CUBE]       =testShapes->nodes[testShapes->getNodeWithName("Cube")]->mesh;
        meshList[MESHID_ICOSPHERE]  =testShapes->nodes[testShapes->getNodeWithName("Icosphere")]->mesh;
        meshList[MESHID_SPACESHIP]  =testShapes->nodes[testShapes->getNodeWithName("Spaceship")]->mesh;
        meshList[MESHID_BLOPPY]     =testShapes->nodes[testShapes->getNodeWithName("Bloppy")]->mesh;

        meshList[MESHID_PWASCEONAIN]=fishes->nodes[fishes->getNodeWithName("Cone 3")]->mesh;
        meshList[MESHID_CHINOSAURE]=fishes->nodes[fishes->getNodeWithName("Cone 3")]->mesh;

        meshList[MESHID_GROUND]     =baseDecor2->nodes[baseDecor2->getNodeWithName("ground")]->mesh;
        meshList[MESHID_BUILDING1]  =baseDecor2->nodes[baseDecor2->getNodeWithName("building1")]->mesh;
        meshList[MESHID_BUILDING2]  =baseDecor2->nodes[baseDecor2->getNodeWithName("building2")]->mesh;
        meshList[MESHID_BUILDING3]  =baseDecor2->nodes[baseDecor2->getNodeWithName("building3")]->mesh;

        return 1;
    }
}