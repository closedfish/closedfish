#include "Header.h"
using namespace std;

/*
* The representation is as follows char p will be K for king N for knight and so on4
* 
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
* even numbers will be a white peice and the following number will be te black version of the piece
* 0 pawn (1 is black pawn)
* 2 knight
* 4 bishop
* 6 rook
* 8 queen
* 10 king 
*/
int get_peice(int i, int j)
{
	return 0;
}

/*
* 1) We need to make sure that we do not go over any peices for any moves (except knights)
* 2) we do not capture any of our own peices
* 3) we do not exit the board
* 4) our king does not come in check
* 5) our king does not stay in check
*/

bool is_move_legal()
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

/*
* makes the mth move for a piece on the square i,j 
* checks if the move is legal
* checks if the opponent is in check stalemated or checkmated
*/
void make_move(int m, int i, int j)
{

}

