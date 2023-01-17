#pragma once

#include <Windows.h>
#include <iostream>
#include <atlimage.h>
#include <utility>
#include <sys/stat.h>

#define MAX_PATH 100

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
		col1 = NULL;
		col2 = NULL;

		ci = CImage();

		files = new char* [64];

		initFileNames();

		wchar_t* fileN = new wchar_t[4];
		fileN[0] = 's';
		fileN[1] = 'q';
		fileN[2] = 's';
		fileN[3] = '\0';

		_wmkdir(fileN);

		delete[] fileN;

		if (type_of_init == false)//this is only for testing on chess.com
		{
			boardL = 304 * scrW / 1920;
			boardT = 105 * scrH / 1080;
			sideW = 104 * 1920 / scrW;
			sideH = 104 * 1080 / scrH;
		}
		else
		{
			//TBD
		}
		minColMatch = 0.9 * sideW * sideH;
		//split_the_board();
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
	/// Does what the name suggests
	/// </summary>
	/// <returns>screen width</returns>
	int giveW()
	{
		return scrW;
	}
	/// Does what the name suggests
	/// </summary>
	/// <returns>screen height</returns>
	int giveH()
	{
		return scrH;
	}
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
	static char* _saveScreenToFile(LPCSTR filename);
	/// <summary>
	/// Creates a bitmap and returns the path to the file
	/// </summary>
	/// <param name="filename">name of the file</param>
	static void saveScreenToFile(LPCSTR filename);
	/// <summary>
	/// Saves current screen to a file, with a given type
	/// </summary>
	/// <param name="filename">Name of the file</param>
	/// <param name="type">Determines how to save the image:
	/// 0 - raw bitmap
	/// 1 - png
	/// 2 - jpeg
	/// default - raw bitmap</param>
	void saveScreenToFileWithType(LPCSTR filename, int type);
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
	WCHAR* _saveScreenToFileWithType(LPCSTR filename, int type);
	/// <summary>
	/// DOES NOT WORK
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="data"></param>
	void writeToFile(LPCSTR filename, char* data);
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
	void getBoardColours(int& col1, int& col2);
	/// <summary>
	/// Removes the background colour
	/// Valid values for the background colour are given by getBoardColours
	/// </summary>
	void removeBgColours(HBITMAP& hbitmap);
	/// <summary>
	/// Testing the removeBgColours method
	/// </summary>
	void testRemoveBg();

	static void _reverseInt(int nr, char*& rev)
	{
		if (nr == 0)
		{
			rev = new char[1];
			rev[0] = '0';
			return;
		}
		int nrDigits = 0;
		int zeros = 0;
		while (nr % 10 == 0 && nr)
		{
			nrDigits++;
			zeros++;
			nr /= 10;
		}
		int _rev = 0;
		while (nr)
		{
			nrDigits++;
			_rev = _rev * 10 + nr % 10;
			nr /= 10;
		}
		int cpos = 0;
		rev = new char[nrDigits];
		while (_rev)
		{
			rev[cpos++] = _rev % 10 + '0';
			_rev /= 10;
		}
		while (zeros)
		{
			rev[cpos++] = '0';
			zeros--;
		}
	};

	void initFileNames()
	{
		char* IHateOpenCVOnGod;
		char* temp;
		for (int i = 0; i < 64; ++i)
		{
			if (10 <= i)
			{
				IHateOpenCVOnGod = new char[13];
				IHateOpenCVOnGod[0] = 's';
				IHateOpenCVOnGod[1] = 'q';
				IHateOpenCVOnGod[2] = 's';
				IHateOpenCVOnGod[3] = '\\';
				_reverseInt(i, temp);
				IHateOpenCVOnGod[4] = 's';
				IHateOpenCVOnGod[5] = 'q';
				IHateOpenCVOnGod[6] = temp[0];
				IHateOpenCVOnGod[7] = temp[1];
				IHateOpenCVOnGod[8] = '.';
				IHateOpenCVOnGod[9] = 'p';
				IHateOpenCVOnGod[10] = 'n';
				IHateOpenCVOnGod[11] = 'g';
				IHateOpenCVOnGod[12] = '\0';
			}
			else
			{
				IHateOpenCVOnGod = new char[12];
				IHateOpenCVOnGod[0] = 's';
				IHateOpenCVOnGod[1] = 'q';
				IHateOpenCVOnGod[2] = 's';
				IHateOpenCVOnGod[3] = '\\';
				_reverseInt(i, temp);
				IHateOpenCVOnGod[4] = 's';
				IHateOpenCVOnGod[5] = 'q';
				IHateOpenCVOnGod[6] = temp[0];
				IHateOpenCVOnGod[7] = '.';
				IHateOpenCVOnGod[8] = 'p';
				IHateOpenCVOnGod[9] = 'n';
				IHateOpenCVOnGod[10] = 'g';
				IHateOpenCVOnGod[11] = '\0';
			}
			files[i] = new char[MAX_PATH];
			GetFullPathNameA(IHateOpenCVOnGod, MAX_PATH, files[i], nullptr);
		}
	}

	void saveAllSquares()
	{
		for (int i = 0; i < 64; ++i)
		{
			ci.Attach(hSquares[i], CImage::DIBOR_DEFAULT);
			ci.Save(files[i], Gdiplus::ImageFormatPNG);
			ci.Detach();
		}
	}

	void findAllPieces()
	{
		memcpy(oldPieces, piece, sizeof(int) * 64);
		split_the_board();
		saveAllSquares();
		//Call to Dimitrije's thing for each square
		/*
		for (int i = 0; i < 64; ++i)
		{
			function(files[i], piece[i]);
		}
		*/
	}

private:

	/// <summary>
	/// snakecase because it might cause a memory leak, or it may be useless
	/// </summary>
	void init_squares();

	//this is a memory leak waiting to happen :/
	HBITMAP hSquares[64];

	//piece types according to postion, 0 is bottom left
	int piece[64], oldPieces[64];

	//filenames
	char** files;

	//variables for different measurments
	int scrW, scrH;
	int boardL, boardT, boardR, boardB;
	int sideW, sideH;
	int minColMatch;
	int col1, col2;

	CImage ci;
};
