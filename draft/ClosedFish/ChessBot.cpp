// ChessBot.cpp
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include <wingdi.h>
#include <atlimage.h>
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
		CREATE_NEW, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

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
		CREATE_NEW, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

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

void bmpClass::init_squares()
{
	for (int i = 0; i < 64; ++i)
	{
		squares[i].bmType = 0;
		squares[i].bmWidth = sideL;
		squares[i].bmHeight = sideL;
		squares[i].bmWidthBytes = squares[i].bmWidth * 4;
		squares[i].bmPlanes = 1;
		squares[i].bmBitsPixel = 32;
		char* bits = (char*)malloc(squares[i].bmWidth * squares[i].bmHeight * 4);
		squares[i].bmBits = bits;
	}
}

void bmpClass::split_the_board()
{
	HDC hdcScreen;
	HDC hdcMemDC = NULL;
	HBITMAP hBmpScreen = NULL;
	BITMAP bmpScreen;

	hdcScreen = GetDC(NULL);

	hdcMemDC = CreateCompatibleDC(hdcScreen);

	RECT rc;
	HWND hwndDesktop;
	HBITMAP hBmpOld = NULL;

	hwndDesktop = GetDesktopWindow();
	GetClientRect(hwndDesktop, &rc);

	hBmpScreen = CreateCompatibleBitmap(hdcScreen, rc.right - rc.left, rc.bottom - rc.top);

	SelectObject(hdcMemDC, hBmpScreen);

	BitBlt(hdcMemDC, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top, hdcScreen, 0, 0, SRCCOPY);

	GetObject(hBmpScreen, sizeof(BITMAP), &bmpScreen);

	unsigned char* bits = (unsigned char*)malloc(bmpScreen.bmWidth * bmpScreen.bmHeight * bmpScreen.bmBitsPixel / 8 + 1);//+1 for safety

	BITMAPINFOHEADER bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bmpScreen.bmHeight;
	bi.biHeight = bmpScreen.bmWidth;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	GetDIBits(hdcScreen, hBmpScreen, 0, bmpScreen.bmHeight, &bits, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	//Clean up
	DeleteObject(hBmpScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	ReleaseDC(hwndDesktop, hdcScreen);

	int BPP = (this->prevBmp.bmBitsPixel / 8);
	unsigned char* buffer = (unsigned char*)malloc(sideL * sideL * BPP);

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			for (int scan = 0; scan < sideL; ++scan)
			{
				int start = this->prevBmp.bmWidth * (this->boardT + i * sideL + scan) * BPP + //vertical offset of board
					(this->boardL + sideL * j) * BPP; //horziontal offset of the board
				memcpy(buffer + scan * sideL * BPP, (bits + start), sideL * BPP);
				hSquares[i * 8 + j] = CreateBitmap(sideL, sideL, (UINT)1, (UINT)prevBmp.bmBitsPixel, buffer);
			}
		}
	}

	*(bits + bmpScreen.bmWidth * bmpScreen.bmHeight * bmpScreen.bmBitsPixel / 8) = '\0';

	std::ofstream fout("test.txt");

	for (int i = 0; i < sideL; ++i)
	{
		for (int j = 0; j < sideL; ++j)
		{
			fout << (int)*(buffer + i * sideL + j)<<" ";
		}
		fout << '\n';
	}

	//deleting the bits
	free(bits);
	free(buffer);
	/*

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			hSquares[i] = CreateCompatibleBitmap(hdcScreen, sideL, sideL);
			hBmpOld = (HBITMAP)SelectObject(hdcMemDC, hSquares[i]);
			BitBlt(hdcMemDC, boardL + j * sideL, boardT + i * sideL,
				sideL, sideL, hdcScreen, 0, 0, SRCCOPY);
			hSquares[i] = (HBITMAP)SelectObject(hdcMemDC, hBmpOld);
			GetObject(hSquares[i], sizeof(BITMAP), &squares[i]);
		}
	}

	//Clean up
	DeleteObject(hBmpOld);
	DeleteObject(hBmpScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	ReleaseDC(hwndDesktop, hdcScreen);
	*/
}


void bmpClass::printSq()
{
	CImage ci = CImage();
	ci.Attach(hSquares[4], CImage::DIBOR_DEFAULT);
	ci.Save(L"lucaESarac.bmp", Gdiplus::ImageFormatBMP);
	ci.Detach();
}

void bmpClass::saveScreenToFileWithType(LPCWSTR filename, int type)
{
	HDC hdcScreen;
	HDC hdcMemDC = NULL;
	HBITMAP hBmpScreen = NULL;
	BITMAP bmpScreen;

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

	CImage ci = CImage();
	ci.Attach(hBmpScreen, CImage::DIBOR_DEFAULT);
	switch (type)
	{
	case 0:
		ci.Save(filename, Gdiplus::ImageFormatBMP);
		break;
	case 1:
		ci.Save(filename, Gdiplus::ImageFormatPNG);
		break;
	case 2:
		ci.Save(filename, Gdiplus::ImageFormatJPEG);
		break;
	default:
		ci.Save(filename, Gdiplus::ImageFormatBMP);
		break;

	}

	//Clean up
	ci.Destroy();
	if (hBmpScreen != nullptr) {
		DeleteObject(hBmpScreen);
	}
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	ReleaseDC(hwndDesktop, hdcScreen);
}


WCHAR* bmpClass::_saveScreenToFileWithType(LPCWSTR filename, int type)
{
	HDC hdcScreen;
	HDC hdcMemDC = NULL;
	HBITMAP hBmpScreen = NULL;
	BITMAP bmpScreen;

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

	CImage ci = CImage();
	ci.Attach(hBmpScreen, CImage::DIBOR_DEFAULT);
	switch (type)
	{
	case 0:
		ci.Save(filename, Gdiplus::ImageFormatBMP);
		break;
	case 1:
		ci.Save(filename, Gdiplus::ImageFormatPNG);
		break;
	case 2:
		ci.Save(filename, Gdiplus::ImageFormatJPEG);
		break;
	default:
		ci.Save(filename, Gdiplus::ImageFormatBMP);
		break;
	}

	WIN32_FIND_DATAW fileData;

	HANDLE hFind = FindFirstFile(filename, &fileData);

	if (hFind = INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	//Clean up
	DeleteObject(hBmpScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	ReleaseDC(hwndDesktop, hdcScreen);
	FindClose(hFind);

	return fileData.cFileName;
}

void bmpClass::writeToFile(LPCWSTR filename, char* data)
{
	HANDLE hf = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
		(DWORD)0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	WriteFile(hf, (LPCVOID)data, 100, 0, NULL);
}
