// ChessBot.cpp
//

#include <iostream>
#include <chrono>
#include <Windows.h>
#include <wingdi.h>
#include "ChessBotUI.h"

const int size_square = 100, top_left_x = 304, top_left_y = 137;

int screen_width, screen_height;

/// <summary>
/// Sends a click to the given position
/// </summary>
/// <param name="posX">The x coordinate, should be in the range 0, screen_width - 1</param>
/// <param name="posY">The Y coordinate, should be in the range 0, screen_height - 1</param>
/// <returns>
/// true if the click was sent, false otherwise
/// </returns>
bool bmpClass::sendClick(int posX, int posY)
{
	SetCursorPos(posX, posY);

	MOUSEINPUT mInp;

	POINT curPos;
	GetCursorPos(&curPos);

	mInp.dx = curPos.x;
	mInp.dy = curPos.y;
	mInp.mouseData = NULL;
	mInp.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
	mInp.dwExtraInfo = NULL;

	INPUT click[1];
	click[0].type = INPUT_MOUSE;
	click[0].mi = mInp;

	int sent = SendInput(1, click, sizeof(click));

	return sent == 1;
}


void bmpClass::send_input(char* inpt)
{
	char c1, c2, l1, l2;
	c1 = *inpt;
	c2 = *(inpt + 2);
	l1 = *(inpt + 1);
	l2 = *(inpt + 3);

	std::cout << c1 << l1 << " " << c2 << l2 << "\n";

	int fx, fy;
	fx = (top_left_x + 50 + size_square * (int)(c1 - 'a')) * (screen_width) / 1920;
	fy = (top_left_y + 50 + size_square * (int)('8' - l1)) * (screen_height) / 1080;

	int sx, sy;
	sx = (top_left_x + 50 + size_square * (int)(c2 - 'a')) * (screen_width) / 1920;
	sy = (top_left_y + 50 + size_square * (int)('8' - l2)) * (screen_height) / 1080;

	sendClick(fx, fy);

	Sleep(50);

	sendClick(sx, sy);
}


void bmpClass::getPrimaryScreen(BITMAP& bmpScreen)
{
	HDC hdcScreen;
	HDC hdcMemDC = NULL;
	HBITMAP hBmpScreen = NULL;

	hdcScreen = GetDC(NULL);

	hdcMemDC = CreateCompatibleDC(hdcScreen);

	RECT rc;
	HWND hwndDesktop;

	hwndDesktop = GetDesktopWindow();
	GetClientRect(hwndDesktop, &rc);

	hBmpScreen = CreateCompatibleBitmap(hdcScreen, rc.right - rc.left, rc.bottom - rc.top);

	SelectObject(hdcMemDC, hBmpScreen);

	BitBlt(hdcMemDC, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top, hdcScreen, 0, 0, SRCCOPY);

	GetObject(hBmpScreen, sizeof(BITMAP), &bmpScreen);

	//Clean up
	DeleteObject(hBmpScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	ReleaseDC(hwndDesktop, hdcScreen);
}

int getSizeChr(char* chr)
{
	char* beg = chr;
	int cnt = 0;
	while (*beg != '\0')
	{
		++cnt;
		++beg;
	}
	return cnt;
}

/// <summary>
/// Creates a bitmap and returns the path to the file
/// </summary>
/// <param name="filename">name of the file</param>
/// <returns>string representing the filepath</returns>
char* bmpClass::_saveScreenToFile(LPCWSTR filename)
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

void bmpClass::saveScreenToFile(LPCWSTR filename)
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

	//Close the handle to the file that was created
	CloseHandle(hf);

	//Clean up
	DeleteObject(hBmpScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	ReleaseDC(hwndDesktop, hdcScreen);
}


void bmpClass::split_the_board()
{
	/*
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			int pos = i * 8 + j;
			char* lpBmp = new char[sideL * sideL];
			for (int scan = 0; scan < sideL; ++scan)
			{
				int curScanLine = boardT - 1 + i * sideL + scan;
				int start = curScanLine * prevBmp.bmWidth + boardL + sideL * j;
				int offset = scan * sideL;
				memcpy(lpBmp + offset, &prevBmp.bmBits + start, sideL);
			}
			//pieces[pos] = *CreateBitmap(sideL, sideL, 1, 32, lpBmp);
		}
	}
	*/

	//This may cause a memory leak
	HDC screenDC;
	HDC memDC = NULL;
	HDC pieceDC = NULL;

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			int pos = i * 8 + j;

		}
	}
}

/*
int main()
{
	if (GetSystemMetrics(SM_CMONITORS) > 1)
	{
		std::cout << "Multiple monitors detected, using the primary one\n";
	}
	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);
	char input[5];
	//int sz;
	//BYTE* screenBmp = getPrimaryScreen(sz);
	LPCWSTR name = L"test2.bmp";
	bmpClass test = bmpClass(false);
	auto start = std::chrono::high_resolution_clock::now();
	char* chr = test.saveScreenToFile(name);//takes ~33ms
	while (*chr != '\0' && chr!=nullptr && (*chr) > 0)
	{
		std::cout << *chr;
		chr++;
	}
	auto _end = std::chrono::high_resolution_clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(_end - start).count();
	std::cout << dur << "\n";
	while (!true)
	{
		std::cin >> input;
		//the input should be of the form clcl
		//for example a1a2 moves a piece form a1 to a2
		//send_input(input);
	}
}
*/
