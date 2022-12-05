#pragma once
#include <Windows.h>

struct piece
{
	/// <summary>
	/// Type of the piece, 0 for pawn, 1 for knight, 2 for bishop, 3 for rook, 4 for queen, 5 for king
	/// </summary>
	int type;
	/// <summary>
	/// The letter in the position
	/// </summary>
	char posL;
	/// <summary>
	/// The number in the position
	/// </summary>
	char posN;
	/// <summary>
	/// true if the piece is taken
	/// </summary>
	bool isTaken;
	/// <summary>
	/// Handle to the bitmap associated with the piece
	/// </summary>
	HBITMAP hbm;
};

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
			sideL += (sideL & 1);
		}
		else
		{
			//TBD
		}
		init_squares();
		split_the_board();
		AllocConsole();
	}
	/// <summary>
	/// Send a click to a given screen position
	/// </summary>
	/// <param name="posX"> x position for the click</param>
	/// <param name="posY"> y position</param>
	/// <returns>true if the click was sent succesfully</returns>
	static bool sendClick(int posX, int posY);
	static void send_input(char* inps);
	static void getPrimaryScreen(BITMAP& bmp);
	/// <summary>
	/// Creates a bitmap and returns the path to the file
	/// </summary>
	/// <param name="filename">name of the file</param>
	/// <returns>string representing the filepath</returns>
	static char* _saveScreenToFile(LPCWSTR filename);
	static void saveScreenToFile(LPCWSTR filename);
	/// <summary>
	/// Saves current screen to a file, with a given type
	/// </summary>
	/// <param name="filename">Name of the file</param>
	/// <param name="type">Determines how to save the image:
	/// 0 - raw bitmap
	/// 1 - png
	/// 2 - jpeg
	/// default - raw bitmap</param>
	void saveScreenToFileWithType(LPCWSTR filename, int type);
	/// <summary>
	/// Saves current screen to a file, with a given type, and return the path to the file
	/// </summary>
	/// <param name="filename">Name of the file</param>
	/// <param name="type">Determines how to save the image:
	/// 0 - raw bitmap
	/// 1 - png
	/// 2 - jpeg
	/// default - raw bitmap</param>
	/// <returns>WCHAR to filepath</returns>
	WCHAR* _saveScreenToFileWithType(LPCWSTR filename, int type);
	/// <summary>
	/// DOES NOT WORK
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="data"></param>
	void writeToFile(LPCWSTR filename, char* data);
	/// <summary>
	/// test
	/// </summary>
	void printSq();

private:
	/// <summary>
	/// Splits the board into 64 bitmaps, one for each square, name uses snakecase because it might cause a memory leak
	/// </summary>
	void split_the_board();

	/// <summary>
	/// snakecase because it might cause a memory leak, or it may be useless
	/// </summary>
	void init_squares();

	//this is a memory leak waiting to happen :/
	BITMAP prevBmp;//somehow this doesn't cause a segfault or memory leak... yet
	BITMAP squares[64];
	HBITMAP hSquares[64];
	piece pieces[32];
	int scrW, scrH;
	int boardL, boardT, sideL;
};
