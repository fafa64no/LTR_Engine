
#include "LTR_Engine_lib.h"

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
#include <chrono>
double get_delta_time(bool readOnly=false);
#include "input.h"

#include "gl_renderer.cpp"
#include "text_renderer.cpp"
#include "render_interface.cpp"
#include "characters.cpp"
#include "game.cpp"

// ############################################################################
//                            Main loop
// ############################################################################
int main(){
    get_delta_time();
    init_debug_log_system();
    SM_TRACE("Loading LTR_Engine");

    //Memory allocation
    SM_TRACE("Allocating memory");
    transientStorage=make_bump_allocator(MB(200));
    persistentStorage=make_bump_allocator(MB(500));

    input=(Input*)bump_alloc(&persistentStorage,sizeof(Input));
    SM_ASSERT(input,"Failed to allocate input");
    RenderInterface::renderData=(RenderInterface::RenderData*)bump_alloc(&persistentStorage,sizeof(RenderInterface::RenderData));
    SM_ASSERT(RenderInterface::renderData,"Failed to allocate renderData");
    RenderInterface::nodeContainer=(RenderInterface::NodeContainer*)bump_alloc(&persistentStorage,sizeof(RenderInterface::NodeContainer));
    SM_ASSERT(RenderInterface::nodeContainer,"Failed to allocate nodeContainer");
    RenderInterface::node2DContainer=(RenderInterface::Node2DContainer*)bump_alloc(&persistentStorage,sizeof(RenderInterface::Node2DContainer));
    SM_ASSERT(RenderInterface::node2DContainer,"Failed to allocate node2DContainer");
    gameData=(GameData*)bump_alloc(&persistentStorage,sizeof(GameData));
    SM_ASSERT(gameData,"Failed to allocate renderData");

    //Window creation
    SM_TRACE("Creating window");
    platform_fill_keycode_lookup_table();
    platform_create_window(1280,720,"LTR Engine");

    //Libs
    SM_TRACE("Initialising OpenGL");
    SM_ASSERT(gl_init(),"Failed to init OpenGL");
    platform_swap_buffers();
    SM_ASSERT(Scenes::SetupScenes(),"Failed to load scenes");

    //Game initialisation
    init_game();

    //Game loop
    SM_TRACE("Updating game");
    float delayBetweenFrames_nanoseconds=1.0f/60.0f;
    while (gameData->is_running){
        float dt=get_delta_time();
        //Update
        platform_update_window();
        update_game(dt);
        gl_render();
        platform_swap_buffers();
        transientStorage.used=0;
        while(get_delta_time(true)<delayBetweenFrames_nanoseconds);
    }
    SM_TRACE("Exiting game");
    return 0;
}

double get_delta_time(bool readOnly){
    static auto lastTime=std::chrono::steady_clock::now();
    auto currentTime=std::chrono::steady_clock::now();
    double delta=std::chrono::duration<double>(currentTime-lastTime).count();
    if(!readOnly)
        lastTime=currentTime;
    return delta;
}
