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

/// <summary>
/// Converts an RGB value to a single int
/// </summary>
/// <param name="R">explains itself</param>
/// <param name="G">explains itself</param>
/// <param name="B">explains itself</param>
/// <returns>the color as a 4-byte integer</returns>
#define _convertRGBToInt(R,G,B) ((int)R + ((int)G << 8) + ((int)B << 16))

#define swapPixels(arr,temp,pos1,pos2) temp = arr[pos1], arr[pos1] = arr[pos2], arr[pos2] = temp; temp = arr[pos1 + 1]; arr[pos1 + 1] = arr[pos2 + 1]; arr[pos2 + 1] = temp; temp = arr[pos1 + 2]; arr[pos1 + 2] = arr[pos2 + 2]; arr[pos2 + 2] = temp; temp = arr[pos1 + 3]; arr[pos1 + 3] = arr[pos2 + 3]; arr[pos2 + 3] = temp;

/// <summary>
/// Converts to 32-bit integers to a 64-bit integer
/// </summary>
#define i64(a,b) a + (static_cast<int_fast64_t>(b) << 32LL)

const unsigned int last32Bits = 4294967295;

/// <summary>
/// namespace with conversion functions, from and to different types
/// </summary>
namespace conv
{
	/// <summary>
	/// tolerance to difference in neighbouring colours
	/// </summary>
	const int tolerance = 2 + (2 << 8) + (2 << 16);
	/// <summary>
	/// Converts an integer to a colour
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

//void bmpClass::_reverseInt(int nr, char*& rev)

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
#if _WIN32 == 1
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
#else
	std::cout << "e pe 64 de biti";
	SetCursorPos(posX, posY);

	MOUSEINPUT mInp;

	POINT curPos;
	GetCursorPos(&curPos);

	mInp.dx = (long long int)curPos.x;
	mInp.dy = (long long int)curPos.y;
	mInp.mouseData = NULL;
	mInp.dwFlags = (long long int)(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
	mInp.dwExtraInfo = NULL;

	INPUT click[1];
	click[0].type = (long long int)INPUT_MOUSE;
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

void getHBMPBits(HBITMAP hBitmap, BYTE*& bits)
{
	HDC hdcMemDC = NULL;

	hdcMemDC = CreateCompatibleDC(NULL);

	SelectObject(hdcMemDC, hBitmap);

	BITMAPINFO bi;

	bi = { 0 };

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	//get the header
	GetDIBits(hdcMemDC, hBitmap, 0, 0, NULL, &bi, DIB_RGB_COLORS);

	//important
	bi.bmiHeader.biCompression = BI_RGB;

	bits = new BYTE[bi.bmiHeader.biSizeImage];

	GetDIBits(hdcMemDC, hBitmap, 0, bi.bmiHeader.biHeight, (LPVOID)bits, &bi, DIB_RGB_COLORS);


	//Clean up
	DeleteObject(hdcMemDC);
	ReleaseDC(NULL, hdcMemDC);
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

	int sideHForImage = sideH * 1.3, sideWForImage = sideW * 1.3;
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
			match = 0;
			for (int scan = 0; scan < sideH; ++scan)
			{
				for (int w = 0; w < sideW; ++w)
				{
					int cpos = bi.bmiHeader.biWidth * (this->boardT + i * sideH + scan) * BPP + //vertical offset of board
						(this->boardL + sideW * j) * BPP + w;//horziontal offset of the board
					int col = _convertRGBToInt(bits[cpos], bits[cpos + 1], bits[cpos + 2]);
					if (col == col1 || col == col2)
					{
						match++;
					}
				}
			}
			if (match >= minColMatch)
			{
				piece[i * 8 + j] = 0;
			}

			memset(buffer, (BYTE)0, bmpSize);

			for (int scan = 0; scan < sideHForImage; ++scan)
			{
				int start = bi.bmiHeader.biWidth * (this->boardT - deltaH + i * sideH + scan) * BPP + //vertical offset of board
					(this->boardL - deltaW + sideW * j) * BPP; //horziontal offset of the board
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
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(start - end).count();
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

#if _DEBUG
	col2 = bestCol;
	byte* bCol1 = conv::_intToRGB(col1);
	byte* bCol2 = conv::_intToRGB(col2);
	std::cout << (int)*bCol1 << " <> " << (int)*(bCol1 + 1) << " <> " << (int)*(bCol1 + 2) << "\n";
	std::cout << (int)*bCol2 << " <> " << (int)*(bCol2 + 1) << " <> " << (int)*(bCol2 + 2) << "\n";


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

WCHAR* bmpClass::_saveScreenToFileWithType(LPCSTR filename, int type)
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

	WIN32_FIND_DATAA fileData;

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

	return (WCHAR*)fileData.cFileName;
}

void bmpClass::writeToFile(LPCSTR filename, char* data)
{
	HANDLE hf = CreateFile(filename, GENERIC_READ | GENERIC_WRITE,
		(DWORD)0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	WriteFile(hf, (LPCVOID)data, 100, 0, NULL);
}
