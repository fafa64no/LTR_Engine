#include "game.h"
#include "assets.h"
#include "zone.h"


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
void init_game(){
    reset_key_bindings(input);
    init_zones();

    gameData->is_running=true;
    gameData->is_paused=false;
    gameData->currentZone=testZone;
}
void update_game(){
    //Exit
    if(key_pressed_this_frame(input->keyBindings[EXIT_KEY]))gameData->is_running=false;
    //Pause
    if(key_pressed_this_frame(input->keyBindings[PAUSE_KEY]))gameData->is_paused=true-gameData->is_paused;
    if(!gameData->is_paused){
        //Movement
        if(key_is_down(input->keyBindings[FORWARD_KEY]))renderData->currentCamera->camPos+=playerSpeed*renderData->currentCamera->camFront;
        if(key_is_down(input->keyBindings[BACKWARD_KEY]))renderData->currentCamera->camPos-=playerSpeed*renderData->currentCamera->camFront;
        if(key_is_down(input->keyBindings[LEFT_KEY]))renderData->currentCamera->camPos-=playerSpeed*glm::normalize(glm::cross(renderData->currentCamera->camFront, renderData->currentCamera->camUp));
        if(key_is_down(input->keyBindings[RIGHT_KEY]))renderData->currentCamera->camPos+=playerSpeed*glm::normalize(glm::cross(renderData->currentCamera->camFront, renderData->currentCamera->camUp));
        if(key_is_down(input->keyBindings[UP_KEY]))renderData->currentCamera->camPos+=playerSpeed*renderData->currentCamera->camUp;
        if(key_is_down(input->keyBindings[DOWN_KEY]))renderData->currentCamera->camPos-=playerSpeed*renderData->currentCamera->camUp;
        renderData->currentCamera->updatePos(renderData->currentCamera->camPos);
    }
}