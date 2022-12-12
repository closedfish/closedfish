#pragma once
#include <Windows.h>
#include <utility>

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
	//HBITMAP hbm;
};

class bmpClass
{
public:
	/// <summary>
	/// Class constructor
	/// This object DOES NOT support reinitialization at the moment
	/// It will cause a crash
	/// </summary>
	/// <param name="type_of_init">true for using opencv, false for testing</param>
	bmpClass(bool type_of_init)
	{
		scrW = GetSystemMetrics(SM_CXSCREEN);
		scrH = GetSystemMetrics(SM_CYSCREEN);
		if (type_of_init == false)//this is only for testing on chess.com
		{
			boardL = 304 * scrW / 1920;
			boardT = 105 * scrH / 1080;
			sideL = 99 * scrH / 1080;
			//sideL += (sideL & 1);
		}
		else
		{
			//TBD
		}
		split_the_board();
	}
	/// <summary>
	/// Default constructor, for some testing
	/// </summary>
	bmpClass() { bmpClass(false); }
	///this causes undefined, unexpected behaviour and will cause a crash
	~bmpClass()
	{
		for (int i = 0; i < 63; ++i)
		{
			DeleteObject(this->hSquares[i]);
			//std::cout << i << "<-\n";
			//hSquares[i] = NULL;
		}
	}//*/
	/// <summary>
	/// Send a click to a given screen position
	/// </summary>
	/// <param name="posX"> x position for the click</param>
	/// <param name="posY"> y position</param>
	/// <returns>true if the click was sent succesfully</returns>
	static bool sendClick(int posX, int posY);
	/// <summary>
	/// old function, pretty useless now
	/// </summary>
	/// <param name="inps">chess inputs, format "a3a4" moves a piece from a3 to a4</param>
	void send_input(char* inps);
	/// <summary>
	/// old function, useless atm
	/// </summary>
	/// <param name="bmp"></param>
	static void getPrimaryScreen(BITMAP& bmp);
	/// <summary>
	/// Creates a bitmap and returns the path to the file
	/// </summary>
	/// <param name="filename">name of the file</param>
	/// <returns>string representing the filepath</returns>
	static char* _saveScreenToFile(LPCWSTR filename);
	/// <summary>
	/// Creates a bitmap and returns the path to the file
	/// </summary>
	/// <param name="filename">name of the file</param>
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
	/// prints a square, mostly for testing
	/// </summary>
	void printSq(int id);
	/// <summary>
	/// Splits the board into 64 bitmaps, one for each square, 
	/// name uses snakecase because it might cause a memory leak or other issuess
	/// </summary>
	void split_the_board();
	/// <summary>
	/// Gives (an educated guess to) the colours of the chessboard.
	/// Chances are this works 100% of the time
	/// </summary>
	/// <param name  = "col1">first colour</param>
	/// <param name  = "col2">second colour</param>
	void getBoardColours(int &col1, int &col2);

private:

	/// <summary>
	/// snakecase because it might cause a memory leak, or it may be useless
	/// </summary>
	void init_squares();

	//this is a memory leak waiting to happen :/
	HBITMAP hSquares[64];

	//piece pieces[32];
	int scrW, scrH;
	int boardL, boardT, sideL;
};
