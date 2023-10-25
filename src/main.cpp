
#include "LTR_Engine_lib.h"
#include "platform.h"
#define APIENTRY
#include "glcorearb.h"

// ############################################################################
//                            Windows Platform
// ############################################################################
#ifdef _WIN32
#include "win32_platform.cpp"
#endif

#include "gl_renderer.h"

int main(){
    platform_create_window(1200,720,(char*)"LTR Engine");
    while (running){
        platform_update_window();
        SM_TRACE("Test trace");
    }
    return 0;
}