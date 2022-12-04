/// <summary>
/// Creates a bitmap and returns the path to the file
/// </summary>
/// <param name="filename">name of the file</param>
/// <returns>string representing the filepath</returns>
#include <Windows.h>
#include <wingdi.h>

char* saveScreenToFile(LPCWSTR filename)
{
    HDC hdcScreen;
    HDC hdcMemDC = NULL;
    HBITMAP hBmpScreen = NULL;
    BITMAP bmpScreen;
    DWORD dwBytesWritten = 0;
    DWORD dwSizeOfDIB = 0;
    HANDLE hf = NULL;
    char* lpBitmap = NULL;
    HANDLE hDIB = NULL;
    DWORD dwBmpSize = 0;

    //Retrieve handle to the primar display
    hdcScreen = GetDC(NULL);

    //Create a compatible DC, which is used in a BitBlt
    hdcMemDC = CreateCompatibleDC(hdcScreen);

    //Get the area of the desktop
    RECT rc;
    HWND hwndDesktop = GetDesktopWindow();
    GetClientRect(hwndDesktop, &rc);

    //Create a compatible bitmap from the screen DC
    hBmpScreen = CreateCompatibleBitmap(hdcScreen, rc.right - rc.left, rc.bottom - rc.top);

    //Select the compatible bitmap into the compatible memory DC
    SelectObject(hdcMemDC, hBmpScreen);

    //Bit Block transfer into the compatible memory DC

    BitBlt(hdcMemDC,
        0, 0,
        rc.right - rc.left, rc.bottom - rc.top,
        hdcScreen,
        0, 0,
        SRCCOPY);

    GetObject(hBmpScreen, sizeof(BITMAP), &bmpScreen);

    BITMAPINFOHEADER bmpInfHdr;

    bmpInfHdr.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfHdr.biHeight = bmpScreen.bmHeight;
    bmpInfHdr.biWidth = bmpScreen.bmWidth;
    bmpInfHdr.biPlanes = 1;
    bmpInfHdr.biBitCount = 32;
    bmpInfHdr.biSizeImage = 0;
    bmpInfHdr.biCompression = BI_RGB;
    bmpInfHdr.biClrImportant = 0;
    bmpInfHdr.biClrUsed = 0;
    bmpInfHdr.biXPelsPerMeter = 0;
    bmpInfHdr.biYPelsPerMeter = 0;

    dwBmpSize = ((bmpScreen.bmWidth * bmpInfHdr.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
    // call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
    // have greater overhead than HeapAlloc
    hDIB = GlobalAlloc(GHND, dwBmpSize);
    lpBitmap = (char*)GlobalLock(hDIB);

    //Get the bits from the bitmap, and copy them into the buffer pointed by lpBitmap
    GetDIBits(hdcScreen, hBmpScreen, 0,
        (UINT)bmpScreen.bmHeight,
        lpBitmap,
        (BITMAPINFO*)&bmpInfHdr, DIB_RGB_COLORS);

    hf = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, (DWORD)0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

    if (hf == INVALID_HANDLE_VALUE);//this is really BAD

    //Add the size of the headers because that's what will be written
    dwSizeOfDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BITMAPFILEHEADER bmpFileHdr;

    //bfType must always be BM for Bitmaps
    bmpFileHdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M

    //size of file
    bmpFileHdr.bfSize = dwSizeOfDIB;

    bmpFileHdr.bfReserved1 = 0;
    bmpFileHdr.bfReserved2 = 0;

    //Offset of the 2 headers
    bmpFileHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

    //The writes
    WriteFile(hf, (LPSTR)&bmpFileHdr, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hf, (LPSTR)&bmpInfHdr, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hf, (LPSTR)lpBitmap, dwBmpSize, &dwBytesWritten, NULL);

    TCHAR path[1000];

    DWORD dwRet;

    dwRet = GetFinalPathNameByHandle(hf, path, 100, VOLUME_NAME_DOS);

    if (dwRet > 1000)
    {
        return nullptr;
    }

    char* ret = new char[dwRet];
    for (int i = 0; i < dwRet; ++i)
    {
        *(ret + i) = *(path + i);
    }

    //Close the handle to the file that was created
    CloseHandle(hf);

    //Clean up
    DeleteObject(hBmpScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
    ReleaseDC(hwndDesktop, hdcScreen);

    return ret;
}