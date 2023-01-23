#include <vector>
#include <array>
#include "Breakthrough2.h"

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

float Breakthrough2::depthval(int n) {
	if (n >= 10) {
		return 200;
	} else {
		return (220 - 20 * n);
	}
}

/*
* blacbox of stockfishs naive eval
*/
int Breakthrough2::SFNeval(CFBoard board) {
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
vector<int> Breakthrough2::mainBreak(CFBoard board, int color) {
	vector<int> line;
	vector<int> ans;
	line.push_back(0);
	line.push_back(0);
	int pruneval = 0;
	if (color) 
		pruneval = 1e6;
	else
		pruneval = -1e6;
	// checks every square for a piece
	for(int sq = 0; sq < 64; sq++) {
		// cerr << sq << '\n';
		// if there is a piece there
		int p = board.getPieceFromCoords(sq);
		if (p>=0 && p%2==0) {
			// if it has a move that intersects with a breakthrough square
			uint64_t moves = board.getLegalMoves(p,sq) & blunderBoard(board, (color+1)%2);
			if (moves > 0) {
				//run the dfs
				line = dfsBreak(board, sq, moves, pruneval);
				if (line[0] > pruneval) {
					ans = line;
					pruneval = ans[0];
				}
			}
		}
	}
	if (ans[0] == 0) {
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
vector<int> Breakthrough2::dfsBreak(CFBoard board, int start, uint64_t ends, int pruneval)
{
	// line of moves
	vector<int> line;
	vector<int> ans;
	// evaluation of the prune
	for (int sq = 0; sq < 64; sq++) {
		//pruned using our alpha beta
		if (((1ll << sq) & ends) > 0) {
			// cerr << "1st move: " << start << ' ' << sq << '\n';
			board.movePiece(start, sq);
			vector<int> temp;
			line = dfsPruneForce(board, 0,1, temp, pruneval);
			// if (start == 43 && sq == 25) {
			// 	cerr << start << ' ' << sq << '\n';
			// 	for (auto x: line)
			// 		cerr << x << ' ';
			// 	cerr << '\n';
			// }
			board.undoLastMove();
			if (line[0] > pruneval) {
				ans = line;
				pruneval = line[0];
			}
			// cerr << start << ' ' << sq << '\n';
			// cerr << line.size() << '\n';
			// for (auto x: line) 
			// 	cerr << x << ' ';
			// cerr << '\n' << board.getRepr();
		}
	}
	// cerr << ans.size() << '\n';
	// for (auto x: ans) {
	// 	cerr << x << ' ';
	// }
	// cerr << '\n';
	if (!ans.size() || ans[0] == 0) {
		vector<int> nob;
		nob.push_back(0);
		return nob;
	}
	return ans;
}

vector<int> Breakthrough2::dfsPruneForce(CFBoard board, int depth, int color, vector<int> v, int eval)
{
	color = color % 2;
	if (depth == 5)	{
		cerr << "Max depth reached\nEval: " << SFNeval(board) << '\n';
		return {SFNeval(board)};
	} else {
	cerr << board.getRepr() << ' ' << color << '\n';
		vector<vector<int>> v2;
		for (int sq = 0; sq < 64; sq++) {
			int p = board.getPieceFromCoords(sq);
			if (p >= 0 && p % 2 == color) {
				uint64_t moves = board.getLegalMoves(p, sq);
				// checks if any piece was captured
				for (int sq2 = 0; sq2 < 64; sq2++) {
					// cerr << sq << ' ' << sq2 << '\n';
					if (((1ll << sq2) & moves) > 0) {
						v.push_back(sq);
						v.push_back(sq2);
						board.movePiece(sq, sq2);
						// cerr << "Before prune: " << color << ' ' << depth << ' ' << sq << ' ' << sq2 << '\n';
						// cerr << "Eval: " << SFNeval(board) << '\n';
						if ((color == 0 && SFNeval(board) < eval - depthval(depth)) || ((color == 1 && SFNeval(board) > eval + depthval(depth)))) {
							// cerr << "Pruned\n";
							board.undoLastMove();
							v.pop_back();
							v.pop_back();
							continue;
						}
						// cerr << "After prune: " << color << ' ' << depth << ' ' << sq << ' ' << sq2 << '\n';
						v2.push_back(dfsPruneForce(board, depth + 1, color + 1, v,eval));
						board.undoLastMove();
						v.pop_back();
						v.pop_back();
						return(minmaxsort(v2, color));
					}
				}
			}
		}
	}
	return {0, 0, 0};
}

vector<int> Breakthrough2::minmaxsort(vector<vector<int>> v, int color)
{
	if (color == 0) {
		int max = v[0][0];
		for (vector<int> v2 : v)
			if (v2[0] > max)
				max = v2[0];
		for (vector<int> v2 : v)
			if (v2[0] >= max - 1)
				if (max - v2[0] == v2[1])
					return v2;
	} else {
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

Closedfish::Move Breakthrough2::getNextMove() {
	vector<int> v = mainBreak(*currentBoard, currentBoard->getCurrentPlayer());
	// cerr << "Ans line size: " << v.size() << '\n';
	// for (auto x: v) {
	// 	cerr << x << ' ';
	// }
	// cerr << '\n';
	if (v.size() <= 2 || v[0] < 1) {
		return make_tuple(0, 0, 0.0);
	} else {
		return make_tuple(v[2], v[3], v[0]);
	}
}

