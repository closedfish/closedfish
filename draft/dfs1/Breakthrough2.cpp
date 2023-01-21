#include <vector>
#include <array>
#include "../../lib/board_implementation/CFBoard.h"
#include "../../lib/weak_pawns/WeakPawns.cpp"
#include "stockfish_naive_eval.h"

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
	return stockfishNaiveEval(board);
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
	line.push_back(0);
	line.push_back(0);
	// checks every square for a piece
	for(int sq = 0; sq < 64; sq++)
	{
		float pruneval = line[0]+line[1]/10;
		// if there is a piece there
		int p = board.getPieceFromCoords(sq);
		if (p>=0 && p%2==0)
		{
			// if it has a move that intersects with a breakthrough square
			uint64_t moves = board.getLegalMoves(p,sq) & blunderBoard(board, (color+1)%2);
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
			float te = line[0] + 0.01 * line[1];
			if (te > pruneval)
			{
				ans = line;
				pruneval = te;
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

vector<int> dfsPruneForce(CFBoard board, int depth, int color, vector<int> v, float eval)
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
			for (int sq = 0; sq < 64; sq++)
			{
				int p = board.getPieceFromCoords(sq);
				if (p >= 0 && p % 2 == color)
				{
					uint64_t moves = board.getLegalMoves(p, sq);
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


int intfromPos(string s)
{
	return 0;
}



vector<int> knightmoves()
{
	vector<int> v = { -21,-19, -10,-6, 6,10,19,21 };
	return v;
}

vector<int> bishopmoves()
{
	vector<int> v;
	for (int i = 1; i < 9; i++)
	{
		v.push_back(-7 * i);
		v.push_back(7 * i);
		v.push_back(-9 * i);
		v.push_back(9 * i);
	}
	return v;
}

vector<int> rookmoves()
{
	vector<int> v;
	for (int i = 1; i < 9; i++)
	{
		v.push_back(-8 * i);
		v.push_back(8 * i);
		v.push_back(-1 * i);
		v.push_back(i);
	}
}

vector<int> queenmoves()
{
	vector<int> v;
	vector<int> v2;
	v = bishopmoves();
	v2 = rookmoves();
	v2.insert(v2.end(), v2.begin(), v.end());
	return v2;
}

vector<int> converter(string a, int color, CFBoard board)
{
	int start = 0;
	int end = 0;
	if (color == 0)
	{
		if (a.length() == 2)
		{
			end = intfromPos(a);
			if (board.getPieceFromCoords(end + 8) == 0)
				start = end + 8;
			else
				start = end + 16;
		}
		else if (a.length() == 3)
		{
			char pie = a[0];
			if (a[2] = '+')
				return (converter(a.substr(1, 2), color, board));
			if (pie != 'O')
				end = intfromPos(a.substr(1, 2));
			else
				end = 62;
			switch(pie)
			{
				case 'N':
				{
					for (int i : knightmoves())
					{
						int sq = end + i;
						if (sq >= 0 && sq <= 63)
						{
							if (board.getPieceFromCoords(sq) == 2)
								start = sq;
						}
					}
					break;
				}
				case 'B':
				{
					for (int i : bishopmoves())
					{
						int sq = end + i;
						if (sq >= 0 && sq <= 63)
						{
							if (board.getPieceFromCoords(sq) == 4)
								start = sq;
						}
					}
					break;
				}
				case 'R':
				{
					for (int i : rookmoves())
					{
						int sq = end + i;
						if (sq >= 0 && sq <= 63)
						{
							if (board.getPieceFromCoords(sq) == 4)
								start = sq;
						}
					}
					break;
				}
				case 'Q':
				{
					for (int i : queenmoves())
					{
						int sq = end + i;
						if (sq >= 0 && sq <= 63)
						{
							if (board.getPieceFromCoords(sq) == 4)
								start = sq;
						}
					}
					break;
				}
				case 'x':
				{
					if (board.getPieceFromCoords(end + 7) == 0)
						start = end + 7;
					else
						start = end + 9;
					break;
				}
				case 'O':
				{
					start = 60;

				}
			}
		}
		else if (a.length() == 4)
		{
			char pie = a[0];
			if (a[2] = '+')
				return (converter(a.substr(1, 3), color, board));
			end = 62;
			switch (pie)
			{
			case 'N':
			{
				bool isRowWise = isdigit(a[1]);
				uint64_t t = (board.getPieceColorBitBoard(2));
				uint64_t knightPlace = ~0ull;
				while (t)
				{
					uint64_t temp = __builtin_ctzll(t);
					t ^= 1ull << temp;
					if (isRowWise) {
						if ((temp >> 3ull) == a[1] - 48) {
							knightPlace = temp;
						}
					}
					else {
						if ((temp & 7ull) == a[1] - 48) {
							knightPlace = temp;
						}
					}
				}
				break;
			}
			case 'B':
			{
				bool isRowWise = isdigit(a[1]);
				uint64_t t = (board.getPieceColorBitBoard(2));
				uint64_t bishopPlace = ~0ull;
				while (t)
				{
					uint64_t temp = __builtin_ctzll(t);
					t ^= 1ull << temp;
					if (isRowWise) {
						if ((temp >> 3ull) == a[1] - 48) {
							bishopPlace = temp;
						}
					}
					else {
						if ((temp & 7ull) == a[1] - 48) {
							bishopPlace = temp;
						}
					}
				}
				break;
			}
			case 'R':
			{
				bool isRowWise = isdigit(a[1]);
				uint64_t t = (board.getPieceColorBitBoard(2));
				uint64_t rookPlace = ~0ull;
				while (t)
				{
					uint64_t temp = __builtin_ctzll(t);
					t ^= 1ull << temp;
					if (isRowWise) {
						if ((temp >> 3ull) == a[1] - 48) {
							rookPlace = temp;
						}
					}
					else {
						if ((temp & 7ull) == a[1] - 48) {
							rookPlace = temp;
						}
					}
				}
				break;
			}
			case 'Q':
			{
				bool isRowWise = isdigit(a[1]);
				uint64_t t = (board.getPieceColorBitBoard(2));
				uint64_t queenPlace = ~0ull;
				while (t)
				{
					uint64_t temp = __builtin_ctzll(t);
					t ^= 1ull << temp;
					if (isRowWise) {
						if ((temp >> 3ull) == a[1] - 48) {
							queenPlace = temp;
						}
					}
					else {
						if ((temp & 7ull) == a[1] - 48) {
							queenPlace = temp;
						}
					}
				}
				break;
			}
			case 'x':
			{
				return (converter(a.substr(1, 3), color, board));
			}
			}
		}
		else if (a.length() == 5)
		{
				char pie = a[0];
				if (pie == 'O')
				{
					start = 60;
					end = 58;
				}
				else
					return (converter(a.substr(1, 4), color, board));

		}
		else if(a.length() == 6)
			return (converter(a.substr(1, 5), color, board));
	}
	vector<int> v;
	v.push_back(start);
	v.push_back(end);
	return v;
}
