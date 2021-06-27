#include <Windows.h>
#include <gdiplus.h>

#include "write.h"

using namespace Gdiplus;

void capture(){
    HDC hdc = GetDC(NULL);
    HDC hDest = CreateCompatibleDC(hdc);

    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    HBITMAP hbDesktop = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hDest, hbDesktop);

    BitBlt(hDest, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    GetImageEncodersSize();

    ReleaseDC(NULL, hdc);
}

int genXLen(LPRECT rect){
    int len;
    
    len = 0;
    return len;
}

int getYLen(LPRECT rect){
    int len;



    len = 0;
    return 0;
}