#include "game.h"
#include "assets.h"


// ############################################################################
//                            Game Constants
// ############################################################################

// ############################################################################
//                            Game Structs
// ############################################################################

// ############################################################################
//                            Game Functions
// ############################################################################
EXPORT_FN void init_game(RenderData* renderDataIn,Input* inputIn){

}
EXPORT_FN void update_game(RenderData* renderDataIn,Input* inputIn){
    if (renderData!=renderDataIn){
        renderData=renderDataIn;
        input=inputIn;
    }
    for (int i=0;i<25;i++){
        int x=i%5;
        int y=(i-i%5)/5;
        draw_sprite(SPRITE_LTR,{x*400.0f,y*200.0f},{200.0f,200.0f});
    }
}