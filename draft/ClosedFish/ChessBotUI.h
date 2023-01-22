#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
#include <Windows.h>
#include <iostream>
#include <atlimage.h>
#include <utility>
#include <future>
#include <sys/stat.h>
#include "class_decl.h"

#define MAX_PATH 100

class bmpClass;

class OpenCV
{
public:
	OpenCV(bmpClass* bm);
	void init(bmpClass* bm);
	void updateScreen(bmpClass* bm);
	void createScaledTemplates(int );
	cv::Mat getTemplates(std::string file);
	cv::Mat conv(cv::Mat& img);
	double detectPieces(cv::Mat& img1, cv::Mat& templ);
	int matchPiece(char* filename);
	std::string getPosition(Board board, int x, int y, Player col);
	void findRects(cv::Mat img, cv::Scalar low, cv::Scalar high, cv::Scalar low1, cv::Scalar high1, COLOR color, COLOR color1, Board board);
	void drawRects(cv::Mat& img);
	std::vector <Tile> addTiles(int size, bool col);
	void initColors(bmpClass* bm);
	std::pair < std::pair<int, int>, std::pair<int, int> > giveBoard(bmpClass* bm);
	char* giveTemplatePath()
	{
		return pathNew;
	}
	void updTemplates(int bg1Old, int bg2Old, int bg1, int bg2);
	void loadTemplates();
	double computePercentage(std::string filename, int side);
	int matchPiecePerc(std::string filename, int side);
	std::string returnFen();
	std::pair<char, int> indexToCords(int i);
	void setColor(bool col)
	{
		playerColor.color = col;
	}
	void resetBoard()
	{
		this->board.Pieces.clear();
	}
	void addPiece(int value, int pos)
	{
		char _type = '*';
		switch (value)
		{
		case 0:
			_type = 'R';
			break;
		case 1:
			_type = 'N';
			break;
		case 2:
			_type = 'B';
			break;
		case 3:
			_type = 'Q';
			break;
		case 4:
			_type = 'K';
			break;
		case 5:
			_type = 'P';
			break;
		case 6:
			_type = 'r';
			break;
		case 7:
			_type = 'n';
			break;
		case 8:
			_type = 'b';
			break;
		case 9:
			_type = 'q';
			break;
		case 10:
			_type = 'k';
			break;
		case 11:
			_type = 'p';
			break;
		}
		if (_type != '*')
		{
			auto _pos = this->indexToCords(pos);
			board.Pieces.push_back(Piece(_type, Tile(_pos.first, _pos.second)));
		}
	}

	int getCol()
	{
		if (playerColor.color == true)
		{
			return 1;
		}
		return 0;
	}
private:
#pragma region variables

	bool use_mask;
	cv::Mat img, templ, mask, result, orig;
	cv::Scalar blacklow, blackhigh, whitelow, whitehigh;

	Player playerColor;
	Board board;
	std::vector < std::pair<char, cv::Mat> > templates;
	char* path = new char[MAX_PATH];
	char* pathNew = new char[MAX_PATH];

	std::vector <Square> rects;

	int max_Trackbar = 5;
#pragma endregion
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
			
		castleL = true, castleR = true;

		if (type_of_init == false)//this is only for testing on chess.com
		{
			boardL = 304 * scrW / 1920;
			boardT = 105 * scrH / 1080;
			sideW = 104 * 1920 / scrW;
			sideH = 104 * 1080 / scrH;
		}
		else
		{
			_opencv = new OpenCV(this);
			auto coord = _opencv->giveBoard(this);
			boardL = coord.first.first;
			boardT = coord.first.second;
			boardR = coord.second.first;
			boardB = coord.second.second;
			sideW = (boardR - boardL) / 8;
			sideH = (boardB - boardT) / 8;
			/*
			_opencv->computePercentage("sq9", sideW);
			_opencv->computePercentage("sq17", sideW);
			_opencv->computePercentage("sq18", sideW);
			_opencv->computePercentage("sq19", sideW);
			_opencv->computePercentage("sq20", sideW);
			_opencv->computePercentage("sq28", sideW);
			_opencv->computePercentage("sq36", sideW);
			_opencv->computePercentage("sq49", sideW);
			_opencv->computePercentage("black_rook", sideW);
			_opencv->computePercentage("sq57", sideW);
			_opencv->computePercentage("sq58", sideW);
			_opencv->computePercentage("sq59", sideW);
			_opencv->computePercentage("sq60", sideW);
			//*/
			this->findAllPieces(true);
			this->returnFen();
		}
		minColMatch = 0.9 * sideW * sideH;
	}
	/// <summary>
	/// Default constructor, for some testing
	/// </summary>
	bmpClass() { bmpClass(false); }
	///this causes undefined, unexpected behaviour and the program will crash
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
	/// Gives the colours of the board as 2 integers
	/// </summary>
	/// <returns>An std pair with the colours</returns>
	std::pair<int, int> giveBoardColours()
	{
		if (col1 == NULL)
		{
			getBoardColours(col1, col2);
		}
		return std::pair<int, int>(col1, col2);
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
	/// <returns>full filepath</returns>
	char* _saveScreenToFileWithType(LPCSTR filename, int type);
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
			if (piece[i] != -1) 
			{
				ci.Attach(hSquares[i], CImage::DIBOR_DEFAULT);
				ci.Save(files[i], Gdiplus::ImageFormatPNG);
				ci.Detach();
			}
		}
	}

	void findAllPieces(bool detColor = false)
	{
		BIGLOCK = true;
		auto start = std::chrono::high_resolution_clock::now();

		memcpy(oldPieces, piece, sizeof(int) * 64);
		split_the_board();
		saveAllSquares();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		bool lock1 = false, lock2 = false, lock3 = false, lock4 = false, lock5 = false, lock6 = false;
		auto ptr = files;
		auto openCVPtr = this->_opencv;
		auto _sideW = this->sideW;

		for (int i = 0; i < 64; ++i)
		{
			if (piece[i] != -1)
			{
				//std::thread sutaLaSuta([this, i]() {
				//piece[i] = _opencv->matchPiecePerc((std::string)files[i], sideW);
				//});
				//sutaLaSuta.detach();
			}
		}

#pragma region MA_ARUNC_PE_GEAM
		//*
		//Call to Dimitrije's thing for each square
		int added = 0;
		auto t1 = std::async(std::launch::async,[this]() {
			for (int i = 0; i < 16; ++i)
			{
				if (piece[i] != -1)
				{
					//std::thread sutaLaSuta([this, i]() {
					piece[i] = _opencv->matchPiecePerc((std::string)files[i], sideW);
					//});
					//sutaLaSuta.detach();
				}
			}
		});
		auto t2 = std::async(std::launch::async,[this]() {
			for (int i = 16; i < 32; ++i)
			{
				if (piece[i] != -1)
				{
					//std::thread sutaLaSuta([this, i]() {
					piece[i] = _opencv->matchPiecePerc((std::string)files[i], sideW);
					//});
					//sutaLaSuta.detach();
				}
			}
		});
		auto t3 = std::async(std::launch::async,[this]() {
			for (int i = 32; i < 48; ++i)
			{
				if (piece[i] != -1)
				{
					//std::thread sutaLaSuta([this, i]() {
					piece[i] = _opencv->matchPiecePerc((std::string)files[i], sideW);
					//});
					//sutaLaSuta.detach();
				}
			}
		});
		auto t4 = std::async(std::launch::async,[this]() {
			for (int i = 48; i < 64; ++i)
			{
				if (piece[i] != -1)
				{
					//std::thread sutaLaSuta([this, i]() {
					piece[i] = _opencv->matchPiecePerc((std::string)files[i], sideW);
					//});
					//sutaLaSuta.detach();
				}
			}
		});
		t1.get();
		t2.get();
		t3.get();
		t4.get();
		//*/

#pragma endregion
		if (detColor == true)
		{
			for (int i = 0; i < 64; ++i)
			{
				if (piece[i] == 4)
				{
					//we re white
					_opencv->setColor(true);
					break;
				}
				if (piece[i] == 10)
				{
					//we re black
					_opencv->setColor(false);
					break;
				}
			}
		}
		_opencv->resetBoard();
		for (int i = 0; i < 64; ++i)
		{
			if (piece[i] != -1)
			{
				_opencv->addPiece(piece[i], i);
			}
		}
		int rookCol = 0 + (1 - _opencv->getCol()) * 6;
		if (piece[0] != rookCol)
		{
			//std::cout << "No more caste left" << _opencv->getCol() << " __ " << piece[0] << " " << rookCol << "\n";
			castleL = false;
		}
		if (piece[7] != rookCol)
		{
			//std::cout << "No more caste right\n";
			castleR = false;
		}
		int kingCol = 4 + (1 - _opencv->getCol()) * 6;
		if (piece[4] != kingCol && _opencv->getCol() == true)
		{
			//std::cout << "No more castels\n";
			castleL = false;
			castleR = false;
		}
		if (piece[3] != kingCol && _opencv->getCol() == false)
		{
			//std::cout << "No more castels\n";
			castleL = false;
			castleR = false;
		}

		BIGLOCK = false;

		auto end = std::chrono::high_resolution_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";

	}

	bool castleLeft()
	{
		return castleL;
	}
	bool castleRight()
	{
		return castleR;
	}

	std::string returnFen()
	{
		std::cout << _opencv->returnFen();
		return _opencv->returnFen();
	}

	bool detectMove()
	{
		findAllPieces();
		for (int i = 0; i < 64; ++i)
		{
			if (piece[i] != oldPieces[i])
			{
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// Stretches the template images to fit the size of the board
	/// </summary>
	void stretchTemplates();

	void updateBgs();

	void changeBg(BYTE* buffer, int _size);

	bool canILookForPieces()
	{
		return !BIGLOCK;
	}

private:

	/// <summary>
	/// snakecase because it might cause a memory leak, or it may be useless
	/// </summary>
	void init_squares();

	/// this is a memory leak waiting to happen :/
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

	int col1Orig = 7771734, col2Orig = 15658706;

	bool castleL = true, castleR = true;

	bool BIGLOCK = false;

	std::vector<int> pieces1, pieces2, pieces3, pieces4, pieces5, pieces6;

	//std::thread t1, t2, t3, t4, t5, t6;

	CImage ci = CImage();

	OpenCV* _opencv = NULL;
};
