
#include "LTR_Engine_lib.h"
#include "input.h"
#include "platform.h"
#define APIENTRY
#define GL_GLEXT_PROTOTYPES
#include "glcorearb.h"

#ifdef _WIN32
#include "win32_platform.cpp"
#endif

#include "gl_renderer.cpp"

int main(){
    BumpAllocator transientStorage=make_bump_allocator(MB(50));
    platform_create_window(1200,720,"LTR Engine");
    input.screenSizeX=1200;
    input.screenSizeY=720;
    SM_TRACE("Initialising OpenGL");
    gl_init(&transientStorage);
    while (running){
        //Update
        SM_TRACE("Updating");
        platform_update_window();
        gl_render();
        platform_swap_buffers();
    }
    SM_TRACE("Out");
    //while(true){Sleep(1);}
    return 0;
}