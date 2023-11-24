
#include "LTR_Engine_lib.h"
#include "input.h"
#include "game.h"
#include "zone.h"

#include "render_interface.h"
#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

// ############################################################################
//                            Platform Includes
// ############################################################################
#include "platform.h"
#ifdef _WIN32
#include "win32_platform.cpp"
#endif

// ############################################################################
//                            Cross Platform Functions
// ############################################################################
#include "gl_renderer.cpp"
#include "scenes.h"
#include "render_interface.cpp"
#include "game.cpp"

// ############################################################################
//                            Main loop
// ############################################################################
int main(){
    init_debug_log_system();
    SM_TRACE("Loading LTR_Engine");

    //Memory allocation
    SM_TRACE("Allocating memory");
    BumpAllocator transientStorage=make_bump_allocator(MB(200));
    BumpAllocator persistentStorage=make_bump_allocator(MB(500));

    input=(Input*)bump_alloc(&persistentStorage,sizeof(Input));
    SM_ASSERT(input,"Failed to allocate input");
    RenderInterface::renderData=(RenderInterface::RenderData*)bump_alloc(&persistentStorage,sizeof(RenderInterface::RenderData));
    SM_ASSERT(RenderInterface::renderData,"Failed to allocate renderData");
    RenderInterface::nodeContainer=(RenderInterface::NodeContainer*)bump_alloc(&persistentStorage,sizeof(RenderInterface::NodeContainer));
    SM_ASSERT(RenderInterface::nodeContainer,"Failed to allocate nodeContainer");
    gameData=(GameData*)bump_alloc(&persistentStorage,sizeof(GameData));
    SM_ASSERT(gameData,"Failed to allocate renderData");

    //Window creation
    SM_TRACE("Creating window");
    platform_fill_keycode_lookup_table();
    platform_create_window(1200,700,"LTR Engine");

    //Libs
    SM_TRACE("Initialising OpenGL");
    gl_init(&transientStorage,&persistentStorage);
    platform_swap_buffers();
    SM_ASSERT(Scenes::SetupScenes(&transientStorage,&persistentStorage),"Failed to load scenes");

    //Game initialisation
    init_game(&transientStorage,&persistentStorage);

    //Game loop
    SM_TRACE("Updating game");
    while (gameData->is_running){
        //Update
        platform_update_window();
        update_game(&transientStorage,&persistentStorage);
        gl_render();
        platform_swap_buffers();
        transientStorage.used=0;
        Sleep(8);
    }
    SM_TRACE("Exiting game");
    return 0;
}
