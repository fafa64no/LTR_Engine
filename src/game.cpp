#include "game.h"
#include "assets.h"
#include "zone.h"
#include "characters.h"


// ############################################################################
//                            Game Constants
// ############################################################################
static float playerSpeed=15.0f;
static float sprintModif=10.0f;
static float blorpSpeed=12.5748f;
static int blorpinator9001;

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
        -500.0f,
        500.0f,
        35.0f
    );
    gameData->freeCam=true;
    RenderInterface::renderData->currentCamera=RenderInterface::freeCam;



    {
        //------Testing glb stuff------//
        std::vector<char>* bufter;
        read_glb_file("assets/meshes/Decor/Test.glb",bufter,transientStorage);
        //read_glb_file("assets/meshes/Creatures/Chinosaure_decimated.glb",bufter,transientStorage);
        sort_glb_file(*bufter);
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

            using namespace RenderInterface;
            using namespace Scenes;
            //Blorpy
            int blorpinator9001=storeNode(new Node(
                glm::vec3(0.0f,-2.725f,0.0f),
                vecToQuat(glm::vec4(0.0f,1.0f,0.0f,3.14159f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_PERSO],
                building1Texture,
                diffuseShader
            ));
            int sdfsdfsfdsdf1=addNodeToRender(nodeContainer->nodes[blorpinator9001]);
            //Props
            int qsdfghjk=storeNode(new Node(
                glm::vec3(-4.0f,-3.0f,0.0f),
                vecToQuat(glm::vec4(0.0f,1.0f,0.0f,-3.14159f/2.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_TUYAU],
                building1Texture,
                diffuseShader
            ));
            int uighghgfgfh=addNodeToRender(nodeContainer->nodes[qsdfghjk]);
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
            int sal10=storeNode(new Node(
                glm::vec3(0.0f,1.0f,17.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_SALLE10],
                building1Texture,
                diffuseShader
            ));
            int sdfsdf2=addNodeToRender(nodeContainer->nodes[sal10]);
            int sal12=storeNode(new Node(
                glm::vec3(0.0f,2.0f,36.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_SALLE12],
                building1Texture,
                diffuseShader
            ));
            int poihjhj=addNodeToRender(nodeContainer->nodes[sal12]);
            int sal14=storeNode(new Node(
                glm::vec3(0.0f,3.0f,57.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_SALLE14],
                building1Texture,
                diffuseShader
            ));
            int sdfg=addNodeToRender(nodeContainer->nodes[sal14]);
            int sal16=storeNode(new Node(
                glm::vec3(0.0f,4.0f,80.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_SALLE16],
                building1Texture,
                diffuseShader
            ));
            int mlkjhy=addNodeToRender(nodeContainer->nodes[sal16]);
            int salCirc=storeNode(new Node(
                glm::vec3(0.0f,-2.0f,110.0f),
                glm::vec4(0.0f,1.0f,0.0f,0.0f),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_SALLERONDE],
                building1Texture,
                diffuseShader
            ));
            int mlkfdsdsfdsfdfsdsfdsfdfsdsfdfsjhy=addNodeToRender(nodeContainer->nodes[salCirc]);
            //Couloirs
            int col1=storeNode(new Node(
                glm::vec3(0.0f,-2.0f,6.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_COULOIR],
                building1Texture,
                diffuseShader
            ));
            int sdjdhgjfhfhj=addNodeToRender(nodeContainer->nodes[col1]);
            int col2=storeNode(new Node(
                glm::vec3(0.0f,-2.0f,10.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_COULOIR],
                building1Texture,
                diffuseShader
            ));
            int hgjf=addNodeToRender(nodeContainer->nodes[col2]);
            int col3=storeNode(new Node(
                glm::vec3(0.0f,-2.0f,24.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_COULOIR],
                building1Texture,
                diffuseShader
            ));
            int jhghjkhjk=addNodeToRender(nodeContainer->nodes[col3]);
            int col4=storeNode(new Node(
                glm::vec3(0.0f,-2.0f,28.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,1.0f),
                meshList[MESHID_COULOIR],
                building1Texture,
                diffuseShader
            ));
            int dfhdfghfg=addNodeToRender(nodeContainer->nodes[col4]);
            int col5=storeNode(new Node(
                glm::vec3(0.0f,-2.0f,46.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,2.0f),
                meshList[MESHID_COULOIR],
                building1Texture,
                diffuseShader
            ));
            int hdjyukto=addNodeToRender(nodeContainer->nodes[col5]);
            int col6=storeNode(new Node(
                glm::vec3(0.0f,-2.0f,68.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,2.0f),
                meshList[MESHID_COULOIR],
                building1Texture,
                diffuseShader
            ));
            int hdjyufghfgkto=addNodeToRender(nodeContainer->nodes[col6]);
            int col7=storeNode(new Node(
                glm::vec3(0.0f,-2.0f,96.0f),
                vecToQuat(glm::vec4(0.0f,0.0f,0.0f,0.0f)),
                glm::vec3(1.0f,1.0f,4.0f),
                meshList[MESHID_COULOIR],
                building1Texture,
                diffuseShader
            ));
            int hdjyufgsdfsdfhfgkto=addNodeToRender(nodeContainer->nodes[col7]);
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
        if(key_is_down(input->keyBindings[TEST_FORWARD_KEY]))RenderInterface::nodeContainer->nodes[blorpinator9001]->translate(glm::vec3(0,0,1)*dt*blorpSpeed*((isSprinting)?sprintModif:1.0f));
        if(key_is_down(input->keyBindings[TEST_BACKWARD_KEY]))RenderInterface::nodeContainer->nodes[blorpinator9001]->translate(-glm::vec3(0,0,1)*dt*blorpSpeed*((isSprinting)?sprintModif:1.0f));
        if(key_is_down(input->keyBindings[TEST_LEFT_KEY]))RenderInterface::nodeContainer->nodes[blorpinator9001]->translate(glm::vec3(1,0,0)*dt*blorpSpeed*((isSprinting)?sprintModif:1.0f));
        if(key_is_down(input->keyBindings[TEST_RIGHT_KEY]))RenderInterface::nodeContainer->nodes[blorpinator9001]->translate(-glm::vec3(1,0,0)*dt*blorpSpeed*((isSprinting)?sprintModif:1.0f));
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
            RenderInterface::renderData->currentCamera->moveDir(camMovDir*sensivity*dt*((isSprinting)?sprintModif:1.0f)*5.0f);
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