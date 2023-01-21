#include <vector>
#include <array>
#include "../../lib/board_implementation/CFBoard.h"
#include "../../lib/weak_pawns/WeakPawns.cpp"

#include <string>
#ifdef _MSC_VER
#include <nmmintrin.h>
#include <immintrin.h>
#define __builtin_popcountll _mm_popcnt_u64
#define __builtin_ctzll _tzcnt_u64
#define __builtin_clzll _lzcnt_u64
#endif

using namespace std;
using namespace WeakPawns;

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

float depthval(int n)
{
	if (n >= 10)
	{
		return 0.2;
	}
	else
	{
		return (2.2 - 0.2 * n);
	}
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
* @param- color 0 for white 1 for black
* @output - the best line for the breakthrough. The first 2 elements of it are 
* the integer and decimal part of the evaluation
* currently only for white 
*/
vector<int> mainBreak(CFBoard board, int color)
{
	vector<int> line;
	vector<int> ans;
	line.push_back(0);
	line.push_back(0);
	int pruneval = 0;
	// checks every square for a piece
	for(int sq = 0; sq < 64; sq++)
	{
		// if there is a piece there
		int p = board.getPieceFromCoords(sq);
		if (p>=0 && p%2==0)
		{
			// if it has a move that intersects with a breakthrough square
			uint64_t moves = board.getLegalMoves(p,sq) & blunderBoard(board, (color+1)%2);
			if (moves > 0)
			{
				//run the dfs
				line = dfsBreak(board, sq, moves, pruneval);
				if (line[0] > pruneval)
				{
					ans = line;
					pruneval = ans[0];
				}
			}
		}
	}
	if (ans[0] == 0)
	{
		vector<int> temp;
		temp.push_back(0);
		return temp;
	}
	return ans;
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
vector<int> dfsBreak(CFBoard board, int start, uint64_t ends, int pruneval)
{
	// line of moves
	vector<int> line;
	vector<int> ans;
	// evaluation of the prune
	for (int sq = 0; sq < 64; sq++)
	{
		//pruned using our alpha beta
		if (((1ll << sq) & ends) > 0)
		{
			board.movePiece(start, sq);
			vector<int> temp;
			line = dfsPruneForce(board, 0,1, temp, pruneval);
			board.undoLastMove();
			if (line[0] > pruneval)
			{
				ans = line;
				pruneval = ans[0];
			}
		}
	}
	if (ans[0] == 0)
	{
		vector<int> nob;
		nob.push_back(0);
		return nob;
	}
	return ans;
}

vector<int> dfsPruneForce(CFBoard board, int depth, int color, vector<int> v, int eval)
{
	color = color % 2;
	if (depth == 5)
	{
		vector<int> tempeval;
		tempeval.push_back(SFNeval(board));
		return tempeval;
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
							board.movePiece(sq, sq2);
							if (SFNeval(board) < eval - depthval(depth))
								continue;
							v2.push_back(dfsPruneForce(board, depth + 1, color + 1, v,eval));
							board.undoLastMove();
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
		int max = v[0][0];
		for (vector<int> v2 : v)
			if (v2[0] > max)
				max = v2[0];
		for (vector<int> v2 : v)
			if (v2[0] >= max - 1)
				if (max - v2[0] == v2[1])
					return v2;
	}
	else
	{
		int min = v[0][0];
		for (vector<int> v2 : v)
			if (v2[0] < min)
				min = v2[0];
		for (vector<int> v2 : v)
			if (v2[0] <= min)
				if (min - v2[0] == v2[1])
					return v2;
	}
}


