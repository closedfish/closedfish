
#include <vector>
#include <array>
#include <iostream>
#include "../../lib/board_implementation/CFBoard.h"
#include <string>

using namespace std;

bool is_protected(int u)
{
	/*
	* @brief: NOT IMPLEMENTED. Checks if pawn is protected
	* @param <int> square index
	* @return if pawn on square is protected
	*/
	return false;
}


int eval()
{
	/*
	* @brief: NOT IMPLEMENTED. evaluates the position might be different from stockfish. to be implemented later on
	* @param Board: <CFBoard> Board 
	* @return naive evaluation of the board
	*/
	return 0;
}

//returns number of possible moves by every piece
int numMoves(int n)
{
	/*
	* @brief: gives total number of possible moves based on the piece index
	* @param n: <int> piece index
	* @return total possible moves
	*/
	if (n % 2 == 1)
		n--;
	if (n == 0)
		return 6;
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

int newTile(int piece, int move)
{
	/*
	* @brief: gives the change in tile of the move
	* @param piece: <int> piece index
	* @param move: <int> move index
	* @return the change in square index
	*/

	// move is invalid
	if (numMoves(piece) < move || move<0)
		return 0;
	// move is valid
	else
	{
		// by piece index
		switch (piece)
		{
			//pawn
			if (piece % 2 == 1)
				return (-1 * newTile(piece - 1, move));
			int k = 0;
			case 0:
				if (move == 1)
					k= - 8;
				else if (move == 2)
					k= - 16;
				else if (move == 3 || move == 5)
					k= - 7;
				else if (move == 4|| move ==6)
					k= - 9;
			// knight
			case 2:
				if (move == 1)
					k = 17;
				else if (move == 2)
					k = 10;
				else if (move == 3)
					k = -6;
				else if (move == 4)
					k = -15;
				else
					k = -1 * k;
			// bishop
			case 4:
				if ( move < 7)
					k = 7 * k;
				else if (move < 14)
					k = -7 * k;
				else if (move < 21)
					k = -9 * k;
				else
					k = 9 * k;
			// rook
			case 6:
				if (move < 7)
					k = -1 * k;
				else if (move < 14)
					k = -8 * k;
				else if (move < 21)
					k = 1 * k;
				else
					k = 8 * k;
			// queen. Just does the corresponding bishop or rook move
			case 8:
				int d = (int)move / 7;
				if (d % 2 == 0)
					return(newTile(4, move - 7 * d / 2));
				else
					move -= 7;
				return (newTile(6, move - 7 * d / 2));
			// king
			case 10:
				if (move == 1)
					k = 7;
				else if (move == 2)
					k = -1;
				else if (move == 3)
					k = -7;
				else if (move == 4)
					k = -8;
				else if (move == 5)
					k = -9;
				else if (move == 6)
					k = 1;
				else if (move == 7)
					k = 9;
				else if (move == 8)
					k = 8;
				else
					k = 2;
			return  k;

		}
	}
}

/*
* the main idea has been optimized a lot more than last week and to that end,
* the DFS has been changed to a BFS and the ideas have been quite a lot
* 
* the main thing to do is to take a position, and keep on making moves for only one side.
* After every 5 moves we can use a naive evaluation criteria and discard all but the top line
* 
* essentially we can represent every legal move as a node and every node will give rise to a new set of nodes and hence our tree
* 
*/

void dfsMain(CFBoard board)
{
	/*
	* @brief: does the DFS
	* @param board: <CFBoard> current board.
	* @return the top 3 lines and their eval
	*/

	// starting best evals and moves
	int eval[3] = { 0,0,0 };
	string play[3] = { "","","" };
	// first dfs gets to dept 5
	dfsHelp(board, "", 1, eval, play);
	// the next part of the dfs goes to depth 15
	for (int i = 0; i < 3; i++)
	{
		//makes 3 boards with the best moves
		CFBoard board1 = board;
		CFBoard board2 = board;
		CFBoard board3 = board;
		//sets everything back to default
		for (int j = 0; j < 3; j++)
		{
			eval[j] = 0;
			play[j] = "";
		}
		//makes the 3 best boards
		board1.makeMovess(play[0]);
		board2.makeMovess(play[1]);
		board3.makeMovess(play[2]);
		//dfs for board 1
		dfsHelp(board1, play[0], 1, eval, play);
		// makes another copy of all the arrays
		int evalc[3] = { 0,0,0 };
		string playc[3] = { "","","" };
		//updates the copy and resets the array
		for (int j = 0; j < 3; j++)
		{
			evalc[j] = eval[j];
			eval[j] = 0;
			playc[j] = play[j];
			play[j] = "";
		}
		//
		dfsHelp(board2, play[1], 1, eval, play);
		for (int j = 0; j < 3; j++)
		{
			update(evalc, playc, eval[j], play[j]);
			eval[j] = 0;
			play[j] = "";
		}
		dfsHelp(board3, play[2], 1, eval, play);
		for (int j = 0; j < 3; j++)
		{
			update(evalc, playc, eval[j], play[j]);
			eval[j] = 0;
			play[j] = "";
		}
	}
}

void update(int* eva, string* play, int e, string moo)
{
	/*
	* @brief: updates the arrays
	* @param eva: <int*> original array of evals
	* @param play: <string*> original array of moves
	* @param e: <int> eval of move to be checked
	* @param moo: <string> string of moves to be checked
	* @return the top 3 lines and their eval
	*/
	if (e > eva[2])
	{
		// we check if the naive eval is good and update it accordingly
		if (e > eva[0])
		{
			eva[2] = eva[1];
			eva[1] = eva[0];
			eva[0] = e;
			play[2] = play[1];
			play[1] = play[0];
			play[0] = moo;
		}
		else if (e > eva[1])
		{
			eva[2] = eva[1];
			eva[1] = e;
			play[2] = play[1];
			play[1] = moo;
		}
		else
		{
			eva[2] = e;
			play[2] = moo;
		}
	}
}

vector<int> allGoodMoves(CFBoard& board, int sq)
{
	/*
	* @brief: makes a vector of all good moves
	* @param board: <CFBoard> current board.
	* @param sq: <int> square index
	* @return the vector of all good moves
	*/
	vector<int> moves;
	int piece = board.getBit(sq);
	int tot = numMoves(piece);
	for (int j = 0; j < tot; j++)
		if (board.isLegal(j))
			moves.push_back(j);
	return moves;
}

void dfsHelp(CFBoard &board, string moo, int depth, int* eva, string* play)
{
	/*
	* @brief: helps the DFS
	* @param board: <CFBoard> current board.
	* @param moo: <string> current string of moves.
	* @param depth: <int> current depth
	* @param eva: <int*> pointer to array of top evaluations
	* @param play: <play*> pointer to corresponding set of moves
	* @return nothing
	*/
	// the actual DFS will start
	if (depth == 5)
	{
		// at depth 5
		update(eva, play, eval(), moo);
	}
	else
	{
		// depth less than 5
		for (int i = 0; i < 64; i++)
		{
			// checking every square for our pieces
			int k = board.getPieceFromCoords(i);
			if (k >= 0 &&  k%2==0)
			{
				// vector with all the moves from this position
				vector<int> moves = allGoodMoves(board, i);
				for (int move : moves)
				{
					// new tile of the move
					int u = i + newTile(k, move);
					//sees if a piece is captured
					int capt = board.getPieceFromCoords(u);
					//updates string storing moves
					moo = moo + board.moveString(i, u);
					//makes the move
					board.makeMove(i,u);
					if (capt >= 0 && is_protected(u))
					{
						// a piece is captured and it was protected so the opponent can and cannot capture then we 
						// run dfs on board where it is and it is not
						CFBoard board2 = board;
						board2.removePiece(u);	
						string moo2 = moo + "X";
						dfsHelp(board2, moo2, depth, eva, play);
					}
					//runs the same thing at greater depth and for every move
					dfsHelp(board, moo, depth+1, eva, play);
					// undos the move
					board.undoMove(u, i, capt);
				}
			}
		}
	}

	
}

