#include <vector>
#include <array>
#include "../../lib/board_implementation/CFBoard.h"
#include <string>

using namespace std;

/* 
* To do a breakthrough we will do a simple version of the alpha beta pruning technique
* Our aim is to completely bruteforce through 1 tree till a depth of about 15 
* pruning the tree only twice before and depths 5 and 10 where we take only the top 3 nodes
* after we get that value we naively evaluate at various positions and check if we go 
* worse than 2x that in which case we prune.
* We are under the assumption that a brekthrough is immediately there
* either by directy capturing or by getting captured
*/


/*
* blackbox for a weak pawns function
*/
uint64_t pawn_pro(CFBoard board)
{
	 uint64_t u;
	 return u;
}

/*
* blacbox of stockfishs naive eval
*/
int SFNeval(CFBoard board)
{
	return 0;
}

/*
* main function for the Breakthrough
* @param- CFboard 
* @output - the best line for the breakthrough. The first 2 elements of it are 
* the integer and decimal part of the evaluation
* currently only for white 
*/
vector<int> mainBreak(CFBoard board)
{
	vector<int> line;
	line.push_back(-10);
	line.push_back(23);
	// checks every square for a piece
	for(int sq = 0; sq < 64; sq++)
	{
		float pruneval = line[0]+line[1]/10;
		// if there is a piece there
		int p = board.getPieceFromCoords(sq);
		if (p>=0 && p%2==0)
		{
			// if it has a move that intersects with a breakthrough square
			uint64_t moves = board.getLegalMoves(p,sq) & pawn_pro(board);
			if (moves > 0)
				//run the dfs
				line = dfsBreak(board, sq,moves,pruneval);
		}
	}
	return line;
}

/*
* @param board: the board
* @param start: tile of the peice to move
* @param ends: all possible moves of that piece
* @param pruneval: evaluation of any previous lines 
* @return: the top line of that piece along with the CF eval using Stockfish in the same vector
* the first 2 elements are used to store the integer and decimal part of the evaluation respectively
* this function calls the DFS functions accoridngly to perform our alpha beta pruning
*/
vector<int> dfsBreak(CFBoard board, int start, uint64_t ends, float pruneval)
{
	// line of moves
	vector<int> line;
	// flag to check if we have completed 1 bruteforce dfs or not
	bool flag = false;
	if (pruneval == -10.23)
		flag = true;
	// evaluation of the prune
	for (int sq = 0; sq < 64; sq++)
	{
		// if its the first one that has to be pruned brutelu
		if (((1ll << sq) & ends)>0 && flag)
		{
			int p = board.getPieceFromCoords(sq);
			board.movePiece(start, sq);
			vector<int> temp;
			line = dfsBruteForce(board, 1, 1, temp);
			board.forceUndo(start, sq, p);
			pruneval = line[0]+0.01*line[1];
			flag = false;
		}
		//pruned using our alpha beta
		if (((1ll << sq) & ends) > 0)
		{
			int p = board.getPieceFromCoords(sq);
			board.movePiece(start, sq);
			vector<int> temp;
			line = dfsPruned(board, 0,1, temp, pruneval);
			board.forceUndo(start, sq, p);
			pruneval = line[0] + 0.01 * line[1];
		}
	}
	return line;
}

/*
* Brute force is a stretch. On avg there are about 30 possible moves per turn and 
* over a depth of 10 (30)^9 is not possible for us to do efficiently.
* Hence at the depth of 4 when there are already about 27000 positions we evaluated we start pruning 
* already at every turn. our choice is pruning a line if it was 2 worse than the pruneval
*/

vector<int> dfsBruteForce(CFBoard board, int depth, int color, vector<int> v)
{
	color = color % 2;
	if (depth == 3)
	{
		float eval = SFNeval(board);
		v.insert(v.begin(),static_cast<int>(eval));
		double decp = (eval - v[0]) * 100;
		v.insert(v.begin(), static_cast<int>(decp));
		return v;
	}
	else
	{
		vector<vector<int>> v2;
		if (color == 0)
		{
			for (int sq = 0; sq < 64; sq++)
			{
				int p = board.getPieceFromCoords(sq);
				if (p >= 0 && p % 2 == color)
				{
					uint64_t moves = board.getLegalMoves(p, sq);
					// checks if any piece was captured
					int capt = 0;
					for (int sq2 = 0; sq2 < 64; sq2++)
					{
						if (((1ll << sq2) & moves) > 0)
						{
							v.push_back(sq);
							v.push_back(sq2);
							int p2 = board.getPieceFromCoords(sq2);
							board.movePiece(sq, sq2);
							v2.push_back(dfsBruteForce(board, depth + 1, color + 1, v));
							board.forceUndo(sq, sq2, p2);
							return(minmaxsort(v2,color));
						}
					}
				}
			}
		}
	}
}

vector<int> dfsPruned(CFBoard board, int depth, int color, vector<int> v, float eval)
{
	color = color % 2;
	if (depth == 5)
	{
		float eval = SFNeval(board);
		v.insert(v.begin(), static_cast<int>(eval));
		double decp = (eval - v[0]) * 100;
		v.insert(v.begin(), static_cast<int>(decp));
		return v;
	}
	else
	{
		vector<vector<int>> v2;
		if (color == 0)
		{
			for (int sq = 0; sq < 64; sq++)
			{
				int p = board.getPieceFromCoords(sq);
				if (p >= 0 && p % 2 == color)
				{
					uint64_t moves = board.getLegalMoves(p, sq);
					// checks if any piece was captured
					int capt = 0;
					for (int sq2 = 0; sq2 < 64; sq2++)
					{
						if (((1ll << sq2) & moves) > 0)
						{
							v.push_back(sq);
							v.push_back(sq2);
							int p2 = board.getPieceFromCoords(sq2);
							board.movePiece(sq, sq2);
							if (SFNeval(board) < eval - 2)
								continue;
							v2.push_back(dfsBruteForce(board, depth + 1, color + 1, v));
							board.forceUndo(sq, sq2, p2);
							return(minmaxsort(v2, color));
						}
					}
				}
			}
		}
	}
}

vector<int> minmaxsort(vector<vector<int>> v, int color)
{
	if (color == 0)
	{
		float max = v[0][0] + 0.01 * v[0][1];
		for (vector<int> v2 : v)
			if (v2[0] > max)
				max = v2[0] + 0.01 * v2[1];
		for (vector<int> v2 : v)
			if (v2[0] >= max - 1)
				if (max - v2[0] == v2[1])
					return v2;
	}
	else
	{
		float min = v[0][0] + 0.01 * v[0][1];
		for (vector<int> v2 : v)
			if (v2[0] < min)
				min = v2[0] + 0.01 * v2[1];
		for (vector<int> v2 : v)
			if (v2[0] <= min)
				if (min - v2[0] == v2[1])
					return v2;
	}
}