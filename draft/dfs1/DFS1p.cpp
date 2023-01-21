#include "Header.h"
#include <vector>
#include <array>
#include <iostream>

using namespace std;

/*
* a pawn has 5 possible moves 
* n = 1 is a normal push
* n = 2 is a push by 2 squares
* n = 3 is a capture on the left diagonal
* n = 4 is a capture on the right diagonal
* n = 5 is en passant
* 
* a knight has 8 possibel moves 
* n = 1 is the move 2 that takes the knight 2 squares behind and 1 square to the left
* after that we rotate clockwise by 1 for every other n
* 
* * a king has 10 possible moves
* n = 1 is the move to the bottom left corner after that move clockwise once. so n = 8 is just below
* n = 9 is for castling short
* n = 10 is for castling long
* 
* a bishop has 28 possible moves
* n = 1 to 7 represent moving n squares in the bottom left direction
* n = 8 to 14 represent moving n-7 squares in the top left direction and so on upto n = 28
* 
* a rook also has 28 possible moves
* n = 1 to 7 are moving n squares in the left
* n = 8 to 14 are moving n-7 squares up and so on clockwise until n = 28 is 7 squares down
* 
* a queen can make upto 56 possible moves
* n = 1 to 7 represent moving n squares in the bottom left diagonal direction
* n = 8 to 14 represent moving n-7 squares to the left direction and so on clockwise up to n = 56 is moving 7 squares down
* 
*/





/*
* takes a coordinate from 0,0 upto 8,8 and returns -1 if the square is empty
* even numbers will be a white peice and the following number will be the black version of the piece
* 0 pawn (1 is black pawn)
* 2 knight
* 4 bishop
* 6 rook
* 8 queen
* 10 king 
*/
int get_piece(int i, int j)
{
	return 0;
}

// the least space taking board 
CFBoard noneBoard;

/*
* 1) We need to make sure that we do not go over any peices for any moves (except knights)
* 2) we do not capture any of our own peices
* 3) we do not exit the board
* 4) our king does not come in check
* 5) our king does not stay in check
*/

bool is_move_legal(CFBoard b, int i, int j, int n)
{
	return false;
}

 
//checks whether the playing side has a legal move or not
bool is_stalemated()
{
	return false;
}

//checks whether the current player is currently in check
bool is_check()
{
	return false;
}

//checks whether the current player is checkmated
bool is_checkmate()
{
	return false;
}

//returns material count for current player
int material_count()
{
	return 0;
}

// returns number of moves the piece at position i,j can make
int piece_moves(int i, int j)
{
	return 0;
}

// returns the coordinates of every peice of type n in an array/deque/vector/list and -1,-1 if its not
std::array<int, 16> get_coords(int n)
{
	std::array<int,16> arr;
	for (int i = 0; i < 10; i++)
	{
		arr[i] = 0;
	}
	return arr;
}
/*
* makes the mth move for a piece on the square i,j 
* check if move is legal
* checks if the opponent is in check stalemated or checkmated
*/
CFBoard make_move(int m, int i, int j, CFBoard b)
{
	CFBoard board;
	return board;
}

CFBoard undo_move(int m, int i, int j, CFBoard b)
{
	CFBoard board;
	return board;
}

//evaluates the position might be different from stockfish. NOT FOR YOSHI.
int eval()
{
	return 0;
}

// returns new boards after each move has been made
std::array<CFBoard,56> new_branch(int l,CFBoard b,int j,int i)
{
	std::array<CFBoard,56> moo;
	for (int k = 0; k < l; k++)
			moo[k] = make_move(k, j, i, b);
	return moo;
}

//returns number of possible moves by every piece
int num_moves(int n)
{
	if (n % 2 == 1)
		n--;
	if (n == 0)
		return 5;
	if (n == 2)
		return 8;
	if (n == 4)
		return 28;
	if (n == 6)
		return 28;
	if (n == 8)
		return 56;
	return 10;
}

/*
* the main idea is to do the DFS like a normal chess engine except letting only one side make moves
* the output will be an array/list/deque/vector
* the first element will be time it takes measured in moves
* the second element  will be the evaluation
* the next elements will be the line with the highest evaluation it went through
* the next task is to maintain a best solution array/list/deque/vector and keep on updating it
* and finding a way to break it currently it breaks at time 10 for just a proof of concept
* This significantly eases the minimax algorithm 
* Also note that because of the negamax algorithm and that the total evaluation should be 0
* we have that the algrithm for black would just be the negative for white
*/
void dfs_helper(CFBoard board, int count)
{
	if (count == 10)
	{
		return;
	}
	for(int i=0;i<32;i++)
	{
		if (board.active[i] == -1)
			break;
		else
		{
			std::array<int, 16> arr = get_coords(board.active[i]);
			for (int j = 0; j < 16; j+=2)
			{
				std::array<CFBoard, 56> moo;
				int piece = get_piece(arr[j],arr[j+1]);
				if (piece == -1)
					break;
				else
					moo = new_branch(num_moves(piece), board, arr[j], arr[j + 1]);
				for (int k = 0; k < 56; k++)
				{
					dfs_helper(moo[k], count + 1);
				}
			}
		}
	}
}

void dfs_main(CFBoard& board)
{
		Stockfish::CommandLine::init(argc, argv);
    Stockfish::UCI::init(Stockfish::Options);
    Stockfish::Tune::init();
    Stockfish::PSQT::init();
    Stockfish::Bitboards::init();
    Stockfish::Position::init();
    Stockfish::Bitbases::init();
    Stockfish::Endgames::init();
    Stockfish::Threads.set(size_t(Stockfish::Options["Threads"]));
    Stockfish::Search::clear(); // After threads are up
    Stockfish::Eval::useNNUE = false;
    // Stockfish::Eval::NNUE::init();
    Stockfish::Position::init(); // if not yet init (should rather be inside the
                                 // main function)
}