#pragma once

#include <CFBoard.h>
#include <WeakPawns.h>
#include <EngineWrapper.h>
#include <stockfish_naive_eval.h>
#include <BitOperations.h>
#include <utils.h>

class Breakthrough2 : public Closedfish::ChessEngine {
public:
	/**
	 * @brief This function returns the next move of the current position.
	 *
	 * @return A tuple (startTile, endTile, eval) representing a move from
	 * startTile to endTile, with the evaluation of eval.
	 */
	Closedfish::Move getNextMove();

	float depthval(int n);
	int SFNeval(CFBoard board);
	std::vector<int> mainBreak(CFBoard board, int color);
	std::vector<int> dfsBreak(CFBoard board, int start, uint64_t ends, int pruneval);
	std::vector<int> dfsPruneForce(CFBoard board, int depth, int color, std::vector<int> v, int eval);
	std::vector<int> minmaxsort(std::vector<std::vector<int>> v, int color);
};