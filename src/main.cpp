


// ############################################################################
//                            Platform Globals
// ############################################################################
static bool running=true;

// ############################################################################
//                            Platform Functions
// ############################################################################
bool platform_create_window(int width, int height, char* title);

// ############################################################################
//                            Windows Platform
// ############################################################################
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

// ############################################################################
//                            Windows Globals
// ############################################################################

// ############################################################################
//                            Platform Implementations
// ############################################################################
bool platform_create_window(int width, int height, char* title){
    HINSTANCE instance=GetModuleHandleW(0);
    WNDCLASSW wc={};
    wc.hIcon            =LoadIcon(instance, IDI_APPLICATION);
    wc.hCursor          =LoadCursor(NULL,IDC_ARROW);
    wc.lpszClassName    =(LPCWSTR)title;
    wc.lpfnWndProc      =DefWindowProcW;
    if (!RegisterClassW(&wc)){
        return false;
    }
    int dwStyle=WS_OVERLAPPEDWINDOW;
    HWND window=CreateWindowExW(
        0,
        (LPCWSTR)title,
        (LPCWSTR)title,
        dwStyle,
        100,
        100,
        width,
        height,
        NULL,
        NULL,
        instance,
        NULL
    );
    if (window==NULL){
        return false;
    }
    ShowWindow(window, SW_SHOW);
    return true;
}
#endif

int main(){
    platform_create_window(1200,720,"LTR Engine");
    while (running){
        //Do stuff
    }
    return 0;
}