#include <Windows.h>
#include <gdiplus.h>
#include <chrono>
#include <stdio.h>
#include <string>
#include <sstream>
#include <thread>

#include "write.h"
#include "config.h"

using namespace Gdiplus;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }
   }
   free(pImageCodecInfo);
   return -1;  // Failure
}

void file_export(HBITMAP bitmap){
    //Turn the bitmap into a file
    Gdiplus::GdiplusStartupInput gdiplus_startup_input;
    ULONG_PTR gdiplus_token;
    Gdiplus::GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, NULL);

    Gdiplus::Bitmap *img = new Gdiplus::Bitmap(bitmap, NULL);
    CLSID clsid;
    int ret_val = GetEncoderClsid(CAPTURE_FORMAT, &clsid);

    //char *output_char;
    //sprintf(output_char, "%s%ld.%s", CAPTURE_PREFIX, std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1), CAPTURE_SUFFIX);

    std::wstring output_string;
    long int epoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    std::wstringstream stream;
    stream << epoch;
    output_string = std::wstring(CAPTURE_PREFIX) + stream.str() + std::wstring(CAPTURE_SUFFIX);

    img->Save(output_string.c_str(), &clsid, NULL);
    delete img;

    Gdiplus::GdiplusShutdown(gdiplus_token);
}

void capture(){
    //To be honest, I don't understand this part. I just copied. Forgive me.

    //Create Display Contexts
    HDC screen_dc = GetDC(NULL);
    HDC dest_dc = CreateCompatibleDC(screen_dc);

    //Get Screen Dimentions
    int image_width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int image_height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    //Make a bitmap image
    HBITMAP bitmap = CreateCompatibleBitmap(screen_dc, image_width, image_height);
    
    //Select the bitmap into the destination display context
    HGDIOBJ img_obj = SelectObject(dest_dc, bitmap);

    //Copy the screen into the bitmap
    BitBlt(dest_dc, 0, 0, image_width, image_height, screen_dc, 0, 0, SRCCOPY);

    file_export(bitmap);

    //Use select again to unlink the bitmap and the destination dc (not sure why you use the same function for that)
    SelectObject(dest_dc, bitmap);

    //Finish up
    DeleteDC(dest_dc);
    ReleaseDC(NULL, screen_dc);
}

void init_captured(){
    auto interval = std::chrono::seconds(CAPTURE_INTERVAL);
    do {
        capture();
        std::this_thread::sleep_for(interval);
    }while (1);

}