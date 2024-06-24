#pragma once

#include "render_interface.h"
#include "input.h"
#include "LTR_Engine_lib.h"
#include "zone.h"

// ############################################################################
//                            Game Structs
// ############################################################################
struct GameData{
    bool is_running;
    bool is_paused;
    bool can_move_mouse;
    bool can_move_mouse_toggled;
    Zones::Biome* currentBiome;
    Zones::Region* currentRegion;
    bool freeCam;
    bool debugMode;
};
static GameData* gameData;

// ############################################################################
//                            Game Functions
// ############################################################################
void update_game();
void init_game();




