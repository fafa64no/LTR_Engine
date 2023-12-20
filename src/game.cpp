#include "game.h"
#include "assets.h"
#include "zone.h"


// ############################################################################
//                            Game Constants
// ############################################################################
static float playerSpeed=8.0f;
static float sprintModif=50.0f;

// ############################################################################
//                            Game Functions
// ############################################################################
void init_game(BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
    reset_key_bindings(input);
    Zones::init_zones(persistentStorage);

    gameData->is_running=true;
    gameData->is_paused=false;
    gameData->can_move_mouse=false;
    gameData->can_move_mouse_toggled=false;
    gameData->currentBiome=Zones::testBiome;
    gameData->currentRegion=Zones::testRegion;
    gameData->currentRegion->AddDraw();
    gameData->debugMode=true;
    RenderInterface::freeCam=new RenderInterface::Camera(
        glm::vec3(0.0f,10.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        0.1f,
        500.0f
    );
    RenderInterface::playerCam=new RenderInterface::Camera(
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        -100.0f,
        100.0f,
        35.0f
    );
    gameData->freeCam=true;
    RenderInterface::renderData->currentCamera=RenderInterface::freeCam;

    {
        //------Testing glb stuff------//
        //std::vector<char>* bufter;
        //read_glb_file("assets/meshes/Decor/baseDecor.glb",bufter,transientStorage);
        //read_glb_file("assets/meshes/Creatures/Chinosaure_decimated.glb",bufter,transientStorage);
        //sort_glb_file(*bufter);
        {
            //------Testing node stuff------//
            /*int bloppyId=RenderInterface::storeNode(RenderInterface::nodeContainer,new RenderInterface::Node(
                glm::vec3(3.0f,1.0f,2.0f),
                glm::vec4(0.0f,0.0f,0.0f,0.0f),
                glm::vec3(1.0f,1.0f,1.0f),
                Scenes::meshList[Scenes::MESHID_BLOPPY],
                faridTexture,
                testShader
            ));
            int bloppyRenderId=RenderInterface::addNodeToRender(RenderInterface::renderData,RenderInterface::nodeContainer->nodes[bloppyId]);*/

            /*int qs=RenderInterface::storeNode(RenderInterface::nodeContainer,new RenderInterface::Node(
                glm::vec3(-4.0f,10.2f,-3.0f),
                glm::vec4(0.0f,0.0f,0.0f,0.0f),
                glm::vec3(5.0f,5.0f,5.0f),
                Scenes::meshList[Scenes::MESHID_PWASCEONAIN],
                faridTexture,
                testShader
            ));
            int fgh=RenderInterface::addNodeToRender(RenderInterface::renderData,RenderInterface::nodeContainer->nodes[qs]);*/

            int sdfsdf=RenderInterface::storeNode(new RenderInterface::Node(
                glm::vec3(0.0f,1.0f,0.0f),
                vecToQuat(glm::vec4(0.0f,1.0f,0.0f,0.78539f)),
                glm::vec3(1.0f,1.0f,1.0f),
                Scenes::meshList[Scenes::MESHID_SALLE1],
                building1Texture,
                diffuseShader
            ));
            int fgsdfsdfsh=RenderInterface::addNodeToRender(RenderInterface::nodeContainer->nodes[sdfsdf]);
        }
    }
}
void update_game(BumpAllocator* transientStorage,BumpAllocator* persistentStorage,float dt){
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
        glm::vec3 movement=glm::vec3(0.0f,0.0f,0.0f);
        bool isSprinting=key_is_down(input->keyBindings[SPRINT_KEY]);
        if(key_pressed_this_frame(input->keyBindings[FREECAM_KEY])){
            gameData->freeCam=1-gameData->freeCam;
            if(gameData->freeCam){
                RenderInterface::renderData->currentCamera=RenderInterface::freeCam;
            }else{
                RenderInterface::renderData->currentCamera=RenderInterface::playerCam;
            }
        }
        if(gameData->freeCam){
            RenderInterface::renderData->currentCamera->moveDir(input->mouseDir*input->sensivity*dt);
            if(key_is_down(input->keyBindings[FORWARD_KEY]))    movement.x+=1.0f;
            if(key_is_down(input->keyBindings[BACKWARD_KEY]))   movement.x-=1.0f;
            if(key_is_down(input->keyBindings[LEFT_KEY]))       movement.z+=1.0f;
            if(key_is_down(input->keyBindings[RIGHT_KEY]))      movement.z-=1.0f;
            if(key_is_down(input->keyBindings[UP_KEY]))         movement.y+=1.0f;
            if(key_is_down(input->keyBindings[DOWN_KEY]))       movement.y-=1.0f;
            RenderInterface::renderData->currentCamera->moveRelativePos(dt*playerSpeed*movement*((isSprinting)?sprintModif:1.0f));
        }else{
            glm::vec2 camMovDir{0};float sensivity=1.0f;
            if(key_is_down(input->keyBindings[LOOKUP_KEY]))     camMovDir.y+=2.0f;
            if(key_is_down(input->keyBindings[LOOKDOWN_KEY]))   camMovDir.y-=2.0f;
            if(key_is_down(input->keyBindings[LOOKLEFT_KEY]))   camMovDir.x+=1.0f;
            if(key_is_down(input->keyBindings[LOOKRIGHT_KEY]))  camMovDir.x-=1.0f;
            RenderInterface::renderData->currentCamera->moveDir(camMovDir*sensivity*dt*((isSprinting)?sprintModif:1.0f));
            if(key_is_down(input->keyBindings[FORWARD_KEY]))    movement.y+=1.0f;
            if(key_is_down(input->keyBindings[BACKWARD_KEY]))   movement.y-=1.0f;
            if(key_is_down(input->keyBindings[LEFT_KEY]))       movement.z+=1.0f;
            if(key_is_down(input->keyBindings[RIGHT_KEY]))      movement.z-=1.0f;
            if(key_is_down(input->keyBindings[UP_KEY]))         movement.x+=0.2f;
            if(key_is_down(input->keyBindings[DOWN_KEY]))       movement.x-=0.2f;
            RenderInterface::renderData->currentCamera->moveRelativePos(dt*playerSpeed*movement*((isSprinting)?sprintModif:1.0f));
        }
        //Rendering
        if(key_pressed_this_frame(input->keyBindings[FULLBRIGHT_KEY])){
            if(gameData->currentBiome!=Zones::fullBrightBiome){
                gameData->currentBiome=Zones::fullBrightBiome;
            }else{
                gameData->currentBiome=Zones::testBiome;
            }
        }
        if(key_pressed_this_frame(input->keyBindings[PIXELATION_KEY])){
            if(RenderInterface::renderData->pixelation==1){
                RenderInterface::renderData->pixelation=4;
            }else{
                RenderInterface::renderData->pixelation=1;
            }
        }
        if(key_is_down(input->keyBindings[ZOOMIN_KEY])){
            RenderInterface::renderData->currentCamera->updateZoom(0.975f);
        }
        if(key_is_down(input->keyBindings[ZOOMOUT_KEY])){
            RenderInterface::renderData->currentCamera->updateZoom(1.025f);
        }
        //Debug
        if(gameData->debugMode){
            SM_TRACE("----------DebugStuff----------");
            RenderInterface::renderData->currentCamera->debugPrint();
            //for(int i=0;i<RenderInterface::renderData->nodeCount;i++)RenderInterface::renderData->nodes_to_render[i]->mesh->DebugTrace();
        }
        if(key_pressed_this_frame(input->keyBindings[DEBUG_KEY])){
            gameData->debugMode=true;
        }else{
            gameData->debugMode=false;
        }
    }
}