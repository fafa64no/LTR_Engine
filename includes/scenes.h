#pragma once

#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "render_interface.h"

namespace Scenes{
    // ############################################################################
    //                            Scene declaration
    // ############################################################################
    static RenderInterface::Scene* testShapes;

    enum mesh_enum{
        MESHID_CUBE,
        MESHID_ICOSPHERE,
        MESHID_SPACESHIP,
        MESHID_BLOPPY,
        MESHID_COUNT
    };
    static RenderInterface::Mesh** meshList;

    // ############################################################################
    //                            Scene build
    // ############################################################################
    int SetupScenes(BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
        //Import scenes
        testShapes=new RenderInterface::Scene("assets/meshes/BasicShapes/testShape.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);

        //Alloc memory
        meshList=(RenderInterface::Mesh**)bump_alloc(persistentStorage,sizeof(RenderInterface::Mesh*)*((int)MESHID_COUNT));

        //Store meshes
        meshList[MESHID_CUBE]       =testShapes->nodes[testShapes->getNodeWithName("Cube")]->mesh;
        meshList[MESHID_ICOSPHERE]  =testShapes->nodes[testShapes->getNodeWithName("Icosphere")]->mesh;
        meshList[MESHID_SPACESHIP]  =testShapes->nodes[testShapes->getNodeWithName("Spaceship")]->mesh;
        meshList[MESHID_BLOPPY]     =testShapes->nodes[testShapes->getNodeWithName("Bloppy")]->mesh;

        return 1;
    }
}