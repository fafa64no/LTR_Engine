#include "game.h"
#include "assets.h"
#include "zone.h"
#include "characters.h"


// ############################################################################
//                            Game Constants
// ############################################################################
static float playerSpeed=15.0f;
static float sprintModif=10.0f;

// Debug
static int currentCam{0};

// ############################################################################
//                            Game Functions
// ############################################################################
void init_game(){
    reset_key_bindings(input);
    Zones::init_zones();

    gameData->is_running=true;
    gameData->is_paused=false;
    gameData->can_move_mouse=false;
    gameData->can_move_mouse_toggled=false;
    gameData->currentBiome=Zones::testBiome;
    gameData->currentRegion=Zones::testRegion;
    gameData->currentRegion->AddDraw();
    gameData->debugMode=true;
    RenderInterface::freeCam=new RenderInterface::Camera(
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        0.1f,
        500.0f
    );
    RenderInterface::playerCam=new RenderInterface::Camera(
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        0.1f,
        500.0f
    );
    gameData->freeCam=true;
    RenderInterface::renderData->currentCamera=RenderInterface::freeCam;

    {
        //------Testing glb stuff------//
        //std::vector<char>* bufter;
        //read_glb_file("assets/meshes/Decor/Test.glb",bufter,transientStorage);
        //read_glb_file("assets/meshes/Creatures/Chinosaure_decimated.glb",bufter,transientStorage);
        //sort_glb_file(*bufter);
        {
            using namespace RenderInterface;
            using namespace Scenes;
            //Salles
            int sal8=storeNode(new Node(
                glm::vec3(0.0f,0.0f,0.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_SALLE8],
                building1Texture,
                diffuseShader
            ));
            int sdfsdf1=addNodeToRender(nodeContainer->nodes[sal8]);
        }
    }
}
void update_game(float dt){
    // Exit game
    if(key_pressed_this_frame(input->keyBindings[EXIT_KEY]))gameData->is_running=false;
    // Pause game
    if(key_pressed_this_frame(input->keyBindings[PAUSE_KEY])){
        gameData->can_move_mouse=true-gameData->can_move_mouse;
        gameData->is_paused=true-gameData->is_paused;
        gameData->can_move_mouse_toggled=true;
    }
    if(!gameData->is_paused){
        // FreeCam
        if(key_pressed_this_frame(input->keyBindings[FREECAM_KEY])){
            gameData->freeCam=1-gameData->freeCam;
            if(gameData->freeCam){
                RenderInterface::renderData->currentCamera=RenderInterface::freeCam;
            }else{
                RenderInterface::renderData->currentCamera=RenderInterface::playerCam;
            }
        }

        // Movement
        glm::vec3 movement=glm::vec3(0.0f,0.0f,0.0f);
        bool isSprinting=key_is_down(input->keyBindings[SPRINT_KEY]);
        if(gameData->freeCam){
            RenderInterface::renderData->currentCamera->moveDir(input->mouseDir*input->sensivity*dt);
            if(key_is_down(input->keyBindings[FORWARD_KEY]))    movement.x+=1.0f;
            if(key_is_down(input->keyBindings[BACKWARD_KEY]))   movement.x-=1.0f;
            if(key_is_down(input->keyBindings[LEFT_KEY]))       movement.z+=1.0f;
            if(key_is_down(input->keyBindings[RIGHT_KEY]))      movement.z-=1.0f;
            if(key_is_down(input->keyBindings[UP_KEY]))         movement.y+=1.0f;
            if(key_is_down(input->keyBindings[DOWN_KEY]))       movement.y-=1.0f;
            RenderInterface::renderData->currentCamera->moveRelativePos(dt*playerSpeed*movement*((isSprinting)?sprintModif:1.0f));
        }

        // Fullbright
        if(key_pressed_this_frame(input->keyBindings[FULLBRIGHT_KEY])){
            if(gameData->currentBiome!=Zones::fullBrightBiome){
                gameData->currentBiome=Zones::fullBrightBiome;
            }else{
                gameData->currentBiome=Zones::testBiome;
            }
        }

        // Debug stuff
        if(gameData->debugMode){
            SM_TRACE("----------DebugStuff----------");
        }
        if(key_pressed_this_frame(input->keyBindings[DEBUG_KEY])){gameData->debugMode=true;}else{gameData->debugMode=false;}
    }
}