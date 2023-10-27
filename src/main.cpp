
#include "LTR_Engine_lib.h"
#include "input.h"
#include "assets.h"
#include "game.h"

#include "render_interface.h"
#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

// ############################################################################
//                            Game DLL Stuff
// ############################################################################
typedef decltype(init_game) init_game_type;
typedef decltype(update_game) update_game_type;
static init_game_type* init_game_ptr;
static update_game_type* update_game_ptr;

// ############################################################################
//                            Cross Platform Functions
// ############################################################################
void reload_dll(BumpAllocator* transientStorage);

// ############################################################################
//                            Platform Includes
// ############################################################################
#include "platform.h"
#ifdef _WIN32
#include "win32_platform.cpp"
#endif

#include "gl_renderer.cpp"

int main(){
    //Debug Log setup
    srand(get_timestamp("game.dll"));
    debug_instance_id=rand();
    {
        std::string strBuffer="";
        strBuffer.append(debug_log_location);
        strBuffer.append("LOG");
        strBuffer.append(std::to_string(debug_instance_id));
        strBuffer.append(".log");
        debug_log_path=(char*)malloc(sizeof(char)*strBuffer.length());
        strcpy(debug_log_path,strBuffer.c_str());
    }
    std::string debug_header="\n------------------------------------------------------";
    debug_header.append("\nDebug log file ");
    debug_header.append(std::to_string(debug_instance_id));
    debug_header.append("\n------------------------------------------------------");
    if (file_exists(debug_log_path)){
        write_file(debug_log_path,"",0);
        append_file(debug_log_path,debug_header);
    }else{
        std::ofstream outfile(debug_log_path);
        outfile<<debug_header<<std::endl;
        outfile.close();
    }
    SM_TRACE("Loading LTR_Engine");
    //Memory allocation
    SM_TRACE("Allocating memory");
    BumpAllocator transientStorage=make_bump_allocator(MB(100));
    BumpAllocator persistentStorage=make_bump_allocator(MB(100));
    input=(Input*)bump_alloc(&persistentStorage,sizeof(Input));
    if (!input){
        SM_ERROR("Failed to allocate input");
    }
    renderData=(RenderData*)bump_alloc(&persistentStorage,sizeof(RenderData));
    if (!renderData){
        SM_ERROR("Failed to allocate renderData");
    }
    //Window creation
    SM_TRACE("Creating window");
    platform_create_window(1200,700,"LTR Engine");
    input->screenSizeX=1200;
    input->screenSizeY=700;
    //Libs and dlls
    SM_TRACE("Initialising OpenGL");
    gl_init(&transientStorage);
    platform_swap_buffers();
    SM_TRACE("Loading dll");
    reload_dll(&transientStorage);
    //Game initialisation
    init_game(renderData,input);
    //Game loop
    SM_TRACE("Updating game");
    while (running){
        //Update
        platform_update_window();
        update_game(renderData,input);
        gl_render();
        platform_swap_buffers();
        transientStorage.used=0;
    }
    SM_TRACE("Out");
    return 0;
}

void init_game(RenderData* renderDataIn,Input* inputIn){
    init_game_ptr(renderDataIn,inputIn);
}
void update_game(RenderData* renderDataIn,Input* inputIn){
    update_game_ptr(renderDataIn,inputIn);
}
void reload_dll(BumpAllocator* transientStorage){
    static void* gameDLL;
    static long long lastEditTimestampGameDLL;
    long long  currentTimestampGameDLL=get_timestamp("game.dll");
    if (currentTimestampGameDLL>lastEditTimestampGameDLL){
        if (gameDLL){
            SM_TRACE("gameDLL found");
            bool freeResult=platform_free_dynamic_library(gameDLL);
            SM_ASSERT(freeResult,"Failed to open the free game.dll");
            gameDLL=nullptr;
            SM_TRACE("Freed game.dll");
        }
        //We have to copy the dll otherwise we can't both open it and write it
        while (!copy_file("game.dll","game_load.dll",transientStorage)){
            Sleep(10);
            SM_WARN("Failed to copy game.dll");
        }
        SM_TRACE("Copied game.dll into game_load.dll");
        gameDLL = platform_load_dynamic_library("game_load.dll");
        SM_ASSERT(gameDLL, "Failed to load game_load.dll");
        init_game_ptr=(init_game_type*)platform_load_dynamic_function(gameDLL,"init_game");
        update_game_ptr=(update_game_type*)platform_load_dynamic_function(gameDLL,"update_game");
        SM_ASSERT(init_game_ptr,"Failed to load init_game function");
        SM_ASSERT(update_game_ptr,"Failed to load update_game function");
        lastEditTimestampGameDLL=currentTimestampGameDLL;
    }
}