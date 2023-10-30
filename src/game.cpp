#include "game.h"
#include "assets.h"


// ############################################################################
//                            Game Constants
// ############################################################################
static float playerSpeed=0.1f;

// ############################################################################
//                            Game Structs
// ############################################################################

// ############################################################################
//                            Game Functions
// ############################################################################
EXPORT_FN void init_game(RenderData* renderDataIn,Input* inputIn){
    if (renderData!=renderDataIn){
        renderData=renderDataIn;
        input=inputIn;
    }
    reset_key_bindings(input);
}
EXPORT_FN void update_game(RenderData* renderDataIn,Input* inputIn){
    if (renderData!=renderDataIn){
        renderData=renderDataIn;
        input=inputIn;
    }
    //Movement
    if (key_is_down(input->keyBindings[FORWARD_KEY]))renderData->currentCamera->camPos+=playerSpeed*renderData->currentCamera->camFront;
    if (key_is_down(input->keyBindings[BACKWARD_KEY]))renderData->currentCamera->camPos-=playerSpeed*renderData->currentCamera->camFront;
    if (key_is_down(input->keyBindings[LEFT_KEY]))renderData->currentCamera->camPos-=playerSpeed*glm::normalize(glm::cross(renderData->currentCamera->camFront, renderData->currentCamera->camUp));
    if (key_is_down(input->keyBindings[RIGHT_KEY]))renderData->currentCamera->camPos+=playerSpeed*glm::normalize(glm::cross(renderData->currentCamera->camFront, renderData->currentCamera->camUp));
    if (key_is_down(input->keyBindings[UP_KEY]))renderData->currentCamera->camPos+=playerSpeed*renderData->currentCamera->camUp;
    if (key_is_down(input->keyBindings[DOWN_KEY]))renderData->currentCamera->camPos-=playerSpeed*renderData->currentCamera->camUp;
    renderData->currentCamera->updatePos(renderData->currentCamera->camPos);
}