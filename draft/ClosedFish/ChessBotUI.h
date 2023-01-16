#pragma once
#include <Windows.h>

class bmpClass
{
public:
	bmpClass(bool type_of_init)
	{
		scrW = GetSystemMetrics(SM_CXSCREEN);
		scrH = GetSystemMetrics(SM_CYSCREEN);
		getPrimaryScreen(prevBmp);
		if (type_of_init == false)//no openCV this is only for testing on chess.com
		{
			boardL = 304 * scrW / 1920;
			boardT = 137 * scrH / 1080;
			sideL = 100 * scrH / 1080;
		}
		else
		{
			//TBD
		}
	}
	static bool sendClick(int posX, int posY);
	static void send_input(char* inps);
	static void getPrimaryScreen(BITMAP& bmp);
	static char* _saveScreenToFile(LPCWSTR name);
	static void saveScreenToFile(LPCWSTR name);

private:
	void split_the_board();

	//this is a memory leak waiting to happen :/
	BITMAP prevBmp;//someway this doesn't cause a segfault... yet
	BITMAP pieces[64];
	int scrW, scrH;
	int boardL, boardT, sideL;
};