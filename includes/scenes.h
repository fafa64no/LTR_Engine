#pragma once

#include "gl_renderer.h"
#include "LTR_Engine_lib.h"
#include "render_interface.h"

namespace Scenes{
    // ############################################################################
    //                            Scene declaration
    // ############################################################################
    using namespace RenderInterface;
    static Scene* testShapes;
    static Scene* fishes;
    static Scene* fishes2;
    static Scene* baseDecor;
    static Scene* baseDecor2;
    static Scene* testRooms;
    static Scene* testChars;

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
        MESHID_SALLE8,
        MESHID_SALLE10,
        MESHID_SALLE12,
        MESHID_SALLE14,
        MESHID_SALLE16,
        MESHID_SALLERONDE,
        MESHID_COULOIR,
        MESHID_ARMOIRE,
        MESHID_TUYAU,
        MESHID_PERSO,
        MESHID_COUNT
    };
    static RenderInterface::Mesh** meshList;

    // ############################################################################
    //                            Scene build
    // ############################################################################
    int SetupScenes(BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
        //Import scenes
        testShapes=new RenderInterface::Scene("assets/meshes/BasicShapes/testShape.glb",GL_STATIC_DRAW,persistentStorage,transientStorage);
        fishes=new RenderInterface::Scene("assets/meshes/Creatures/Poisson2.glb",GL_STATIC_DRAW,persistentStorage,transientStorage);
        //fishes2= new RenderInterface::Scene("assets/meshes/Creatures/Chinosaure_decimated.glb",GL_STATIC_DRAW,persistentStorage,transientStorage);
        baseDecor=new RenderInterface::Scene("assets/meshes/Decor/baseDecor.glb",GL_STATIC_DRAW,persistentStorage,transientStorage);
        baseDecor2=new RenderInterface::Scene("assets/meshes/Decor/baseDecor2.glb",GL_STATIC_DRAW,persistentStorage,transientStorage);
        testRooms=new Scene("assets/meshes/Decor/Test.glb",GL_STATIC_DRAW,persistentStorage,transientStorage);
        testChars=new Scene("assets/meshes/Characters/Perso.glb",GL_STATIC_DRAW,persistentStorage,transientStorage);

        //Alloc memory
        meshList=(RenderInterface::Mesh**)bump_alloc(persistentStorage,sizeof(RenderInterface::Mesh*)*((int)MESHID_COUNT));

        //Store meshes
        SM_TRACE("Storing meshes");
        meshList[MESHID_CUBE]       =testShapes->nodes[testShapes->getNodeWithName("Cube")]->mesh;
        meshList[MESHID_ICOSPHERE]  =testShapes->nodes[testShapes->getNodeWithName("Icosphere")]->mesh;
        meshList[MESHID_SPACESHIP]  =testShapes->nodes[testShapes->getNodeWithName("Spaceship")]->mesh;
        meshList[MESHID_BLOPPY]     =testShapes->nodes[testShapes->getNodeWithName("Bloppy")]->mesh;

        meshList[MESHID_PWASCEONAIN]=fishes->nodes[fishes->getNodeWithName("Cone 3")]->mesh;
        meshList[MESHID_CHINOSAURE] =fishes->nodes[fishes->getNodeWithName("Cone 3")]->mesh;

        meshList[MESHID_GROUND]     =baseDecor2->nodes[baseDecor2->getNodeWithName("ground")]->mesh;
        meshList[MESHID_BUILDING1]  =baseDecor2->nodes[baseDecor2->getNodeWithName("building1")]->mesh;
        meshList[MESHID_BUILDING2]  =baseDecor2->nodes[baseDecor2->getNodeWithName("building2")]->mesh;
        meshList[MESHID_BUILDING3]  =baseDecor2->nodes[baseDecor2->getNodeWithName("building3")]->mesh;

        meshList[MESHID_SALLE8]     =testRooms->nodes[testRooms->getNodeWithName("Salle_8")]->mesh;
        meshList[MESHID_SALLE10]    =testRooms->nodes[testRooms->getNodeWithName("Salle_10")]->mesh;
        meshList[MESHID_SALLE12]    =testRooms->nodes[testRooms->getNodeWithName("Salle_12")]->mesh;
        meshList[MESHID_SALLE14]    =testRooms->nodes[testRooms->getNodeWithName("Salle_14")]->mesh;
        meshList[MESHID_SALLE16]    =testRooms->nodes[testRooms->getNodeWithName("Salle_16")]->mesh;
        meshList[MESHID_SALLERONDE] =testRooms->nodes[testRooms->getNodeWithName("Salle_ronde")]->mesh;
        meshList[MESHID_COULOIR]    =testRooms->nodes[testRooms->getNodeWithName("Couloir")]->mesh;
        meshList[MESHID_ARMOIRE]    =testRooms->nodes[testRooms->getNodeWithName("Armoire")]->mesh;
        meshList[MESHID_TUYAU]      =testRooms->nodes[testRooms->getNodeWithName("Tuyau")]->mesh;

        meshList[MESHID_PERSO]      =testChars->nodes[testChars->getNodeWithName("Perso")]->mesh;

        return 1;
    }
}