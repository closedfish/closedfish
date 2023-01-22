#pragma once

#include <BitOperations.h>
#include <CFBoard.h>
#include <EngineWrapper.h>
#include <Heatmap.h>
#include <WeakPawns.h>
#include <algorithm>
#include <array>
#include <queue>
#include <tuple>
#include <vector>
#include <iostream>

class DFS1P : public Closedfish::ChessEngine {
public:
	/**
	 * @brief This function returns the next move of the current position.
	 *
	 * @return A tuple (startTile, endTile, eval) representing a move from
	 * startTile to endTile, with the evaluation of eval.
	 */
	Closedfish::Move getNextMove();

	/**
	 * @brief This function takes tile coordinates and return whether that square
	 * is attacked by opponent pawns or not
	 *
	 * @param currentTurn : <bool> equal to 0 for white, 1 for black.
	 * @param opponentPawnBoard : <uint64_t> equal to the uint64_t board
	 * representation of opponent pawns.
	 * @param row : <int> board row (0 for 8th rank, 7 for 1st rank).
	 * @param col : <int> board column (0 for a file, 7 for h file).
	 *
	 * @return A boolean value representing (row, col) is attacked by opponent
	 * pawns or not.
	 */
	bool squareSafeFromOpponentPawns(const bool &currentTurn,
																	 const uint64_t &opponentPawnBoard,
																	 const int &row, const int &col);

	/**
	 * @brief This function takes a CFBoard, a halfPieceId representing a piece, a
	 * startTile and return a 64-sized array of turn distance from startTile to
	 * other tiles on the board, with respect to the piece. (a.k.a. how many turns
	 * it takes for that piece to go to other tiles).
	 *
	 * @param board : <CFBoard> current board.
	 * @param halfPieceId : <int> equal to half of pieceId (0: pawn, 1: knight, 2:
	 * bishop, 3: rook, 4: queen, 5: king).
	 * @param startTile : <int> starting tile (0: a8, 1: a7, ..., 64: h1).
	 *
	 * @return A 64-sized array of relative distance from startTile to each tile.
	 */
	std::array<int, 64>
	distFromTileToTilesAsPiece(CFBoard &board, int halfPieceId, int startTile, uint64_t& opponentPanwsAttackingBoard);

	/**
	 * @brief This function takes a CFBoard, a heatMap and returns the "distance"
	 * from the board to that heatMap.
	 *
	 * @param board : <CFBoard> current board.
	 * @param heatMap : <int[6][8][8]> heatMap of the current board.
	 *
	 * @return An integer represents the "distance".
	 */
	int distFromHeatmap(CFBoard &board, int (&heatMap)[6][8][8], uint64_t& opponentPanwsAttackingBoard);

	/**
* @brief This function takes a pointer to a CFBoard, the current depth, the
maximum depth, the current line, all possible lines, and performs DFS to search
all possible next moves.
	*
* @param currentBoard : <CFBoard*> current board.
* @param depth : <int> current depth in the DFS.
	* @param maxDepth : <int> maximum depth in the DFS.
	* @param curLine : <vector<tuple<int, int, float>> a vector representing the
current line we are searching
	* @param possibleLines : <vector<vector<tuple<int, int, float>>>> a vector
storing all lines that we have searched.
*/
	void DFS1pAux(CFBoard *currentBoard, int depth, int maxDepth,
								std::vector<Closedfish::Move> curLine,
								std::vector<std::vector<Closedfish::Move>> &possibleLines,
								uint64_t& opponentPanwsAttackingBoard);

	void testDFS();
};