#include "game.h"
#include "assets.h"
#include "zone.h"


// ############################################################################
//                            Game Constants
// ############################################################################
static float playerSpeed=0.1f;

// ############################################################################
//                            Game Functions
// ############################################################################
void init_game(BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
    reset_key_bindings(input);
    init_zones();

    gameData->is_running=true;
    gameData->is_paused=false;
    gameData->can_move_mouse=false;
    gameData->can_move_mouse_toggled;
    gameData->currentZone=testZone;

    {
        //------Testing glb stuff------//
        //std::vector<char>* bufter;
        //read_glb_file("assets/meshes/BasicShapes/testShape.glb",bufter,transientStorage);
        //read_glb_file("assets/meshes/Creatures/Poisson1.glb",bufter,transientStorage);
        //sort_glb_file(*bufter);

        RenderInterface::testScene=new RenderInterface::Scene("assets/meshes/Creatures/Poisson1.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);
        //RenderInterface::testScene=new RenderInterface::Scene("assets/meshes/BasicShapes/testShape.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);
        //RenderInterface::testScene=new RenderInterface::Scene("assets/meshes/Vehicles/Password.glb",GL_DYNAMIC_DRAW,persistentStorage,transientStorage);
    }
    {
        //------Testing node stuff------//
        int bloppyId=RenderInterface::storeNode(RenderInterface::nodeContainer,new RenderInterface::Node(
            glm::vec3(3.0f,1.0f,2.0f),
            glm::vec4(0.0f,0.0f,0.0f,0.0f),
            glm::vec3(1.0f,1.0f,1.0f),
            Scenes::meshList[Scenes::MESHID_BLOPPY],
            faridTexture,
            testShader
        ));
        int bloppyRenderId=RenderInterface::addNodeToRender(RenderInterface::renderData,RenderInterface::nodeContainer->nodes[bloppyId]);
    }
}
void update_game(BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
    //Exit
    if(key_pressed_this_frame(input->keyBindings[EXIT_KEY]))gameData->is_running=false;
    //Pause
    if(key_pressed_this_frame(input->keyBindings[PAUSE_KEY])){
        gameData->can_move_mouse=true-gameData->can_move_mouse;
        gameData->is_paused=true-gameData->is_paused;
        gameData->can_move_mouse_toggled=true;
    }
    if(!gameData->is_paused){
        //Movement
        if(key_is_down(input->keyBindings[FORWARD_KEY]))RenderInterface::renderData->currentCamera->camPos+=playerSpeed*RenderInterface::renderData->currentCamera->camFront;
        if(key_is_down(input->keyBindings[BACKWARD_KEY]))RenderInterface::renderData->currentCamera->camPos-=playerSpeed*RenderInterface::renderData->currentCamera->camFront;
        if(key_is_down(input->keyBindings[LEFT_KEY]))RenderInterface::renderData->currentCamera->camPos-=playerSpeed*glm::normalize(glm::cross(RenderInterface::renderData->currentCamera->camFront,RenderInterface::renderData->currentCamera->camUp));
        if(key_is_down(input->keyBindings[RIGHT_KEY]))RenderInterface::renderData->currentCamera->camPos+=playerSpeed*glm::normalize(glm::cross(RenderInterface::renderData->currentCamera->camFront,RenderInterface::renderData->currentCamera->camUp));
        if(key_is_down(input->keyBindings[UP_KEY]))RenderInterface::renderData->currentCamera->camPos+=playerSpeed*RenderInterface::renderData->currentCamera->camUp;
        if(key_is_down(input->keyBindings[DOWN_KEY]))RenderInterface::renderData->currentCamera->camPos-=playerSpeed*RenderInterface::renderData->currentCamera->camUp;
        RenderInterface::renderData->currentCamera->updatePos(RenderInterface::renderData->currentCamera->camPos);
        //Light
        if(key_pressed_this_frame(input->keyBindings[FULLBRIGHT_KEY])){
            if(gameData->currentZone==testZone){
                gameData->currentZone=fullBrightZone;
            }else{
                gameData->currentZone=testZone;
            }
        }
    }
}