// ChessBot.cpp
//
#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <Windows.h>
#include <wingdi.h>
#include <atlimage.h>
#include <wincodec.h>
#include <utility>
#include <queue>
#include <algorithm>
#include "ChessBotUI.h"

#pragma region macros
/// <summary>
/// Converts an RGB value to a single int
/// </summary>
/// <returns>the color as a 4-byte integer</returns>
#define _convertRGBToInt(R,G,B) ((int)R | ((int)G << 8) | ((int)B << 16))

#define swapPixels(arr,temp,pos1,pos2) temp = arr[pos1], arr[pos1] = arr[pos2], arr[pos2] = temp; temp = arr[pos1 + 1]; arr[pos1 + 1] = arr[pos2 + 1]; arr[pos2 + 1] = temp; temp = arr[pos1 + 2]; arr[pos1 + 2] = arr[pos2 + 2]; arr[pos2 + 2] = temp; temp = arr[pos1 + 3]; arr[pos1 + 3] = arr[pos2 + 3]; arr[pos2 + 3] = temp;

#define swapCols(R, G, B, col) R = col & 255, G = (col>>8) & 255, B = (col>>16)&255

/// <summary>
/// Converts to 32-bit integers to a 64-bit integer
/// </summary>
#define i64(a,b) a + (static_cast<int_fast64_t>(b) << 32LL)

const unsigned int last32Bits = 4294967295;//would be a macro but giving a type makes everything more stable

namespace templateVals2 {
	char* templateNames[24] = { "\\black_king\0",
		"\\b_black_king\0",
		"\\black_queen\0",
		"\\b_black_queen\0",
		"\\black_knight\0",
		"\\b_black_knight\0",
		"\\black_bishop\0",
		"\\b_black_bishop\0",
		"\\black_rook\0",
		"\\b_black_rook\0",
		"\\black_pawn\0",
		"\\b_black_pawn\0",
		"\\white_king\0",
		"\\w_white_king\0",
		"\\white_queen\0",
		"\\b_white_queen\0",
		"\\white_knight\0",
		"\\b_white_knight\0",
		"\\white_bishop\0",
		"\\b_white_bishop\0",
		"\\white_rook\0",
		"\\b_white_rook\0",
		"\\white_pawn\0",
		"\\b_white_pawn\0"
	};
}
#pragma endregion

/// <summary>
/// namespace with conversion functions, from and to different types
/// </summary>
namespace conv
{
	/// <summary>
	/// tolerance to difference in neighbouring colours
	/// might not be used
	/// </summary>
	const int tolerance = 2 + (2 << 8) + (2 << 16);
	/// <summary>
	/// Converts an integer to an RGB colour
	/// </summary>
	/// <param name="colour">The colour, as an int</param>
	// <returns>a pointer to 3 bytes, whicih are the RGB colours</returns>
	byte* _intToRGB(int colour)
	{
		byte* color = new byte[3];
		color[0] = (colour & 255);
		colour = (colour >> 8);
		color[1] = (colour & 255);
		colour = (colour >> 8);
		color[2] = (colour & 255);
		return color;
	}
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

char** templateNames = new char*[1];
int nrTemplates = 1;

void bmpClass::stretchTemplates()
{
	return;
	templateNames[0] = "test\\test.png\0";

	HDC hdc = CreateCompatibleDC(NULL);
	HDC hdcStretch = CreateCompatibleDC(hdc);

	HBITMAP hBmpTemp = NULL;
	HBITMAP hBmpStretch = NULL;

	for (int i = 0; i < nrTemplates; ++i)
	{
		hBmpTemp = (HBITMAP)ci.Load(templateNames[i]);

		SelectObject(hdc, hdcStretch);

		SetStretchBltMode(hdcStretch, HALFTONE);

		hBmpStretch = CreateCompatibleBitmap(hdcStretch, sideW, sideH);

		SelectObject(hdcStretch, hBmpStretch);

		StretchBlt(hdcStretch, 0, 0, sideW, sideH, hdc, 0, 0, 104, 104, SRCCOPY);

		ci.Detach();

		ci.Attach(hBmpStretch, CImage::DIBOR_DEFAULT);

		ci.Save("test\\test2.png", Gdiplus::ImageFormatPNG);

		ci.Detach();
	}

	DeleteObject(hBmpTemp);
	DeleteObject(hBmpStretch);
	DeleteDC(hdc);
	DeleteDC(hdcStretch);
}

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
#ifdef _WIN64
	SetCursorPos(posX, posY);

	MOUSEINPUT mInp;

	POINT curPos;
	GetCursorPos(&curPos);

	mInp.dx = (long long int)curPos.x;
	mInp.dy = (long long int)curPos.y;
	mInp.mouseData = (long long int)NULL;
	mInp.dwFlags = 0x00000002 | 0x00000004;
	mInp.dwExtraInfo = (long long int)NULL;

	INPUT click[1];
	click[0].type = (long long int)INPUT_MOUSE;
	click[0].mi = mInp;

	int sent = SendInput((UINT)1, click, sizeof(click));

	return sent == 1;
#else
	//for some reason this is the always the active block
	//but adding 64-bit compatibility will make this safer
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
#endif
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
	fx = (boardL + 50 + sideW * (int)(c1 - 'a')) * (scrW) / 1920;
	fy = (boardT + 50 + sideH * (int)('8' - l1)) * (scrH) / 1080;

	int sx, sy;
	sx = (boardL + 50 + sideW * (int)(c2 - 'a')) * (scrW) / 1920;
	sy = (boardT + 50 + sideH * (int)('8' - l2)) * (scrH) / 1080;

	sendClick(fx, fy);

	Sleep(150);

	sendClick(sx, sy);

	Sleep(150);
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

char* bmpClass::_saveScreenToFile(LPCSTR filename)
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

void bmpClass::saveScreenToFile(LPCSTR filename)
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
{}

/// <summary>
/// Copies screen bits into buffer, and bitmapinfo 
/// </summary>
/// <param name="bits">bits for the screen</param>
/// <param name="bi">bitmapinfo header</param>
void getScreenBits(BYTE*& bits, BITMAPINFO& bi)
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

	bi = { 0 };

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	//get the header
	GetDIBits(hdcScreen, hBmpScreen, 0, 0, NULL, &bi, DIB_RGB_COLORS);

	//better to be safe
	bi.bmiHeader.biCompression = BI_RGB;

	bits = new BYTE[bi.bmiHeader.biSizeImage + 1];

	GetDIBits(hdcScreen, hBmpScreen, 0, bi.bmiHeader.biHeight, (LPVOID)bits, &bi, DIB_RGB_COLORS);


	//Clean up
	DeleteObject(&bmpScreen);
	DeleteObject(hBmpScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	ReleaseDC(hwndDesktop, hdcScreen);
}

void _getHBMPBits(HBITMAP hBitmap, BYTE* buffer, BITMAPINFO& bi)
{
	HDC hdc = CreateCompatibleDC(NULL);

	bi = { 0 };

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	GetDIBits(hdc, hBitmap, 0, 0, NULL, &bi, DIB_RGB_COLORS);

	//better to be safe
	bi.bmiHeader.biCompression = BI_RGB;

	buffer = new BYTE[bi.bmiHeader.biSizeImage];

	GetDIBits(hdc, hBitmap, 0, bi.bmiHeader.biHeight, (LPVOID)&buffer, &bi, DIB_RGB_COLORS);

	ReleaseDC(NULL, hdc);
}

void bmpClass::split_the_board()
{

	if (this->col1 == NULL)
	{
		this->getBoardColours(this->col1, this->col2);
	}

	auto start = std::chrono::high_resolution_clock::now();

	BYTE* bits;
	BITMAPINFO bi;
	getScreenBits(bits, bi);

	int sideHForImage = sideH * 1.0, sideWForImage = sideW * 1.0;
	int deltaH = sideHForImage - sideH, deltaW = sideWForImage - sideW;

	int BPP = bi.bmiHeader.biBitCount / 8;
	BYTE* buffer = new BYTE[sideWForImage * sideHForImage * BPP];
	int match;

	int bmpSize = sideWForImage * sideHForImage * BPP;

	HBITMAP temp;

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			//checking if the current square has a piece or not
			int pos = ((this->scrH - this->boardB) + i * this->sideH + this->sideH / 6) * bi.bmiHeader.biWidth * BPP + (this->boardL + this->sideW / 2 + this->sideW * j) * BPP;
			int col = _convertRGBToInt(bits[pos], bits[pos + 1], bits[pos + 2]);
			int difW = (bits[pos] - 82) + (bits[pos + 1] - 83) + (bits[pos + 2] - 86);
			int difB = (-bits[pos] + 248) + (-bits[pos + 1] + 248) + (-bits[pos + 2] + 248);

			
			if (abs(difW) > 4 && abs(difB) > 4)
			{
				piece[i*8 + j] = -1;
			}

			else
			{
				piece[i * 8 + j] = 1000;
			}
			

			//if (match >= minColMatch)
			//{
				//std::cout << i * 8 + j << " ";
				//piece[i * 8 + j] = -1;
			//}
			//std::cout << "\n";

			memset(buffer, (BYTE)0, bmpSize);

			for (int scan = 0; scan < sideHForImage; ++scan)
			{
				int start = bi.bmiHeader.biWidth * ((this->scrH - this->boardB) + i * this->sideH + scan) * BPP + //vertical offset of board
					(this->boardL + this->sideW * j) * BPP; //horziontal offset of the board
				if (scan == sideHForImage)
				{
					memcpy(buffer, (bits + start), sideWForImage * BPP);
				}
				else
				{
					//flip the image vertically so it is the correct orientation
					memcpy(buffer + (sideHForImage - scan - 1) * BPP * sideWForImage, (bits + start), sideWForImage * BPP);
				}

				DeleteObject(hSquares[i * 8 + j]);

				hSquares[i * 8 + j] = CreateBitmap(sideWForImage, sideHForImage, (UINT)bi.bmiHeader.biPlanes, (UINT)bi.bmiHeader.biBitCount, (void*)buffer);

				//detect pieces here
			}
		}
	}

	//deleting the bits
	delete[] bits;
	delete[] buffer;

	auto end = std::chrono::high_resolution_clock::now();
	//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(start - end).count();
}

void bmpClass::printSq(int id)
{
	if (id < 0)
	{
		id = 0;
	}
	if (id > 63)
	{
		id = 63;
	}
	if (!hSquares[id])
	{
		split_the_board();
	}
	if (hSquares[id])
	{
		try
		{
			CImage ci = CImage();
			std::cout << "init ";
			ci.Attach(hSquares[id], CImage::DIBOR_DEFAULT);
			std::cout << " attach ";
			ci.Save("lucaESarac.png", Gdiplus::ImageFormatPNG);
			std::cout << " saved\n";
			ci.Detach();
		}
		catch (const std::exception ex)
		{
			std::cout << ex.what() << "\n";
		}
	}
	else
	{
		std::cout << "tot e null";
	}
}

void bmpClass::getBoardColours(int& col1, int& col2)
{
	BYTE* screenBits;
	BITMAPINFO bi;

	getScreenBits(screenBits, bi);

	//std::cout << bi.bmiHeader.biBitCount << "<----\n\n\n\n";
	//std::cout << *screenBits << " " << *(screenBits + 1) << " " << *(screenBits + 2) << " " << *(screenBits + 3) << " " << *(screenBits + 4) << " ";

	int** screenArr = new int* [bi.bmiHeader.biHeight];

	for (int i = 0; i < bi.bmiHeader.biHeight; ++i)
	{
		screenArr[i] = new int[bi.bmiHeader.biWidth];
		memset(screenArr[i], -1, bi.bmiHeader.biWidth * sizeof(int));
	}

	std::vector<int> colours;//colours which are in "appropiate" squares
	//an "appropiate" square is a square of size ~1/128 of the screen size (64 squares which may take ~1/2 of the screen)

	double scrRatio = GetSystemMetrics(SM_CXSCREEN) / 1920.0;

	//actual expected range for the size of the square, divided by 15, to account for probable errors
	int sqSizeMax = ((GetSystemMetrics(SM_CXSCREEN) * GetSystemMetrics(SM_CYSCREEN)) >> 7);
	int sqSizeMin = ((GetSystemMetrics(SM_CXSCREEN) * GetSystemMetrics(SM_CYSCREEN)) >> 9);

	int starts = 0;

	int maxSqSz = -1;

	std::deque< int_fast64_t > positions;

	for (int i = 0; i < bi.bmiHeader.biHeight; ++i)
	{
		for (int j = 0; j < bi.bmiHeader.biWidth; ++j)
		{
			if (screenArr[i][j] == -1)
			{
				starts++;
				int topLeftX = i, topLeftY = j;
				positions.push_back(i64(i, j));
				int curColour = _convertRGBToInt(screenBits[4 * (i * bi.bmiHeader.biWidth + j)],
					screenBits[4 * (i * bi.bmiHeader.biWidth + j) + 1], screenBits[4 * (i * bi.bmiHeader.biWidth + j) + 2]);

				int corCol = 0;//the number of correct colour squares in the detected area, this is for ignoring a border
				int curSqSz = 1;

				while (!positions.empty())
				{
					int_fast64_t cpos = positions.front();
					positions.pop_front();
					int Xc, Yc;
					Xc = cpos & last32Bits;
					Yc = (cpos >> 32);
					if (Xc < 0 || Xc > bi.bmiHeader.biHeight - 1 ||
						Yc < 0 || Yc > bi.bmiHeader.biWidth - 1)
					{
						continue;
					}
					if (screenArr[Xc][Yc] != -1)
					{
						continue;
					}
					int scrCol = _convertRGBToInt(screenBits[4 * (Xc * bi.bmiHeader.biWidth + Yc)],
						screenBits[4 * (Xc * bi.bmiHeader.biWidth + Yc) + 1],
						screenBits[4 * (Xc * bi.bmiHeader.biWidth + Yc) + 2]);

					if (curColour != scrCol)
					{
						continue;
					}

					corCol++;
					screenArr[Xc][Yc] = scrCol;

					if (curSqSz < (Xc - topLeftX + 1) * (Yc - topLeftY + 1))
						//this means there is a more bottom right square of the same colour
					{
						curSqSz = (Xc - topLeftX + 1) * (Yc - topLeftY + 1);
					}

					//Only going in the bottom & right directions since it's searching for the bottom-right corner of the square/rectangle
					positions.push_front(i64(Xc + 1LL, Yc));
					positions.push_front(i64(Xc, Yc + 1LL));
				}

				if (corCol * 1.1 > curSqSz)// this means it's mostly a solid colour
				{
					if (sqSizeMax > curSqSz && sqSizeMin < curSqSz)//it's of the right size
					{
						colours.push_back(_convertRGBToInt(screenBits[4 * (i * bi.bmiHeader.biWidth + j)],
							screenBits[4 * (i * bi.bmiHeader.biWidth + j) + 1], screenBits[4 * (i * bi.bmiHeader.biWidth + j) + 2]));
					}
				}
			}
		}
	}

	std::sort(colours.begin(), colours.end());
	int curApp = 0, curCol = 0;
	int bestApp = -1, bestCol = -1;

	//find first colour with most appearences
	for (auto it : colours)
	{
		if (curCol != it)
		{
			if (bestApp < curApp)
			{
				bestApp = curApp;
				bestCol = curCol;
			}
			curCol = it;
			curApp = 1;
			continue;
		}
		curApp++;
	}
	col1 = bestCol;
	bestApp = 0;
	curCol = -1;
	curApp = 0;
	colours.push_back(-1);
	for (auto it : colours)
	{
		if (it == col1)//completely ignoring the first colour
			continue;
		if (curCol != it)
		{
			if (bestApp < curApp)
			{
				bestApp = curApp;
				bestCol = curCol;
			}
			curCol = it;
			curApp = 1;
			continue;
		}
		curApp++;
	}
	col2 = bestCol;

#if _DEBUG
	//std::cout << col1 << " " << col2 << "\n";
	byte* bCol1 = conv::_intToRGB(col1);
	byte* bCol2 = conv::_intToRGB(col2);
	//std::cout << (int)*bCol1 << " <> " << (int)*(bCol1 + 1) << " <> " << (int)*(bCol1 + 2) << "\n";
	//std::cout << (int)*bCol2 << " <> " << (int)*(bCol2 + 1) << " <> " << (int)*(bCol2 + 2) << "\n";


	//Clean up
	delete[] bCol1;
	delete[] bCol2;
#endif
	delete[] screenArr;
	delete[] screenBits;
}

void bmpClass::removeBgColours(HBITMAP& hbitmap)
{
	if (col1 == NULL)
	{
		this->getBoardColours(col1, col2);
	}

	BITMAPINFO bi;

	bi = { 0 };

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	HDC hdc = CreateCompatibleDC(NULL);

	GetDIBits(hdc, hbitmap, 0, 0, NULL, &bi, DIB_RGB_COLORS);

	//NEVER DELETE THIS or else really bad crashes happen
	//And mad memory leaks
	bi.bmiHeader.biCompression = BI_RGB;

	BYTE* bits = new BYTE[bi.bmiHeader.biSize];

	GetDIBits(hdc, hbitmap, 0, bi.bmiHeader.biHeight, (LPVOID)bits, &bi, DIB_RGB_COLORS);

	int bitsSize = bi.bmiHeader.biSize;

	if (bi.bmiHeader.biBitCount == 24)
	{
		//I make the if statement this way because I expect 32bpp always, this is just to be extra safe
		BYTE* bits2 = new BYTE[bi.bmiHeader.biSize * 4 / 3];
		int cpos = 0;
		bitsSize = bi.bmiHeader.biSize * 4 / 3;
		for (int i = 0; i < bi.bmiHeader.biSize; i += 3)
		{
			bits2[cpos] = bits[i];
			bits2[cpos + 1] = bits[i + 1];
			bits2[cpos + 2] = bits[i + 2];
			bits2[cpos + 3] = 0;
			cpos += 4;
		}
		delete[] bits;
		bits = new BYTE[bitsSize];
		memcpy(bits, bits2, bitsSize);
		delete[] bits2;
	}

	for (int i = 0; i < bitsSize; i += 4)
	{
		int curColour = _convertRGBToInt(bits[i],
			bits[i + 1], bits[i + 2]);
		if (curColour == col1 || curColour == col2)//it's a match
		{
			//white pieces will always have a dark border
			//so removing the background should make it red and not something else
			//hopefully it all goes smooth
			bits[i] = 255;
			bits[i + 1] = 0;
			bits[i + 2] = 0;
		}
	}
	//hbitmap = CreateBitmap()
}

void bmpClass::changeBg(BYTE* bits, int _size)
{

	for (int i = 0; i < _size; i += 4)
	{
		//std::cout << _convertRGBToInt(bits[i + 1], bits[i + 2], bits[i + 3]) << " <> " << (int)bits[i] << " " << (int)bits[i + 1] << " " << (int)bits[i + 4] << " " << (int)bits[i+3] << " <> " << col1Orig << " " << col2Orig << "\n";
		if (_convertRGBToInt(bits[i], bits[i + 1], bits[i + 2]) == col1Orig)
		{
			swapCols(bits[i], bits[i + 1], bits[i + 2], col1);
		}
		else
			if (_convertRGBToInt(bits[i], bits[i + 1], bits[i + 2]) == col2Orig)
			{
				swapCols(bits[i], bits[i + 1], bits[i + 2], col2);
			}
		//bits[i] = 255;
		//bits[i + 1] = 255;
	}
}

void bmpClass::updateBgs()
{
	HBITMAP _template = NULL;

	BITMAP bmp;

	Gdiplus::Bitmap* image(NULL);

	char* templPath = _opencv->giveTemplatePath();
	int sz = sizeof(templPath);

	BYTE* buffer = new BYTE[40000];

	for (int i = 0; i < 24; ++i)
	{
		std::string __path = (std::string)templPath + (std::string)templateVals2::templateNames[i] + (std::string)".png";
		std::string __path2 = (std::string)templPath + (std::string)templateVals2::templateNames[i] + (std::string)"7.png";

		//char* _path = new char[__path.length() + 1];

		int size = MultiByteToWideChar(CP_UTF8, 0, __path.c_str(), -1, NULL, 0);
		WCHAR* _wpath = new WCHAR[size];
		MultiByteToWideChar(CP_UTF8, 0, __path.c_str(), -1, _wpath, size);
		
		size = MultiByteToWideChar(CP_UTF8, 0, __path2.c_str(), -1, NULL, 0);
		WCHAR* _wpath2 = new WCHAR[size];
		MultiByteToWideChar(CP_UTF8, 0, __path2.c_str(), -1, _wpath2, size);

		//strcpy(_path, __path.c_str());

		char* _path2 = new char[__path2.length() + 1];

		strcpy(_path2, __path2.c_str());

		image = Gdiplus::Bitmap::FromFile(_wpath);

		std::cout << "status: " << image->GetHBITMAP(NULL, &_template) << "\n";

		BITMAPINFO _bi;

		_getHBMPBits(_template, buffer, _bi);

		int BPP = _bi.bmiHeader.biBitCount / 8;

		int bmpSize = _bi.bmiHeader.biWidth * _bi.bmiHeader.biHeight * BPP;

		if (i == 1)
		{
			ofstream fout("out.txt");
			for (int i = 0; i < bmpSize; ++i)
			{
				fout << (int)buffer[i] << " ";
			}
		}

		std::cout << ">" << bmpSize << " " << _bi.bmiHeader.biSizeImage << "< " << "\n";

		changeBg(buffer, bmpSize);

		CImage ci;

		ci.Attach(_template, CImage::DIBOR_DEFAULT);

		ci.Save(_path2, Gdiplus::ImageFormatPNG);

		//image = Gdiplus::Bitmap::FromHBITMAP(_template, NULL);

		//std::cout << "new status: " << image->Save(_T(_wpath2), &Gdiplus::ImageFormatPNG) << "\n";

		std::cout << "5 ";

		/*HRESULT hr = _ci.Save(("templatesNew" + '\\' + 't' + (char)(i + 15) + '.' + 'p' + 'n' + 'g'), Gdiplus::ImageFormatBMP);

		if (hr != S_OK)
		{
			DWORD dw = GetLastError();

			LPVOID lpMsgBuf;

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);
			std::cout << (LPCTSTR)lpMsgBuf << "<-\n";
		}*/
	}
}

void bmpClass::testRemoveBg()
{
	HBITMAP _new = (HBITMAP)CopyImage(hSquares[1], IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	removeBgColours(_new);
	CImage _ci;
	_ci.Attach(hSquares[1], CImage::DIBOR_DEFAULT);
	_ci.Save("TestRemoveBg.bmp", Gdiplus::ImageFormatBMP);
	//ci.Destroy();
	DeleteObject(_new);
}

void bmpClass::saveScreenToFileWithType(LPCSTR filename, int type)
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

char* bmpClass::_saveScreenToFileWithType(LPCSTR filename, int type)
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

	char* _path = new char[MAX_PATH];

	GetFullPathNameA(filename, MAX_PATH, _path, nullptr);

	//Clean up
	DeleteObject(hBmpScreen);
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcScreen);
	ReleaseDC(hwndDesktop, hdcScreen);

	return _path;
}

void bmpClass::writeToFile(LPCSTR filename, char* data)
{
	HANDLE hf = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
		(DWORD)0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	WriteFile(hf, (LPCVOID)data, 100, 0, NULL);
}
