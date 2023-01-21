#pragma once
#include <CFBoard.h>
#include <iostream>
#include <tuple>

namespace Closedfish {
// Gives you the information on next move
typedef std::tuple<int, int, float> Move;

/**
 * @brief Abstract class from which all of our algorithms are gonna be derived
 */
class ChessEngine {
public:
	/**
	 * @brief Abstract constructor, shouldn't be run directly
	 */
	ChessEngine(){};

	/**
	 * @brief Abstract destructor, shouldn't be run directly
	 */
	~ChessEngine(){};

	/**
	 * @brief Sets the pointer to the current board instance (necessary)
	 *
	 * @param <CFBoard*> : A pointer to the current board instance.
	 *
	 */
	void setBoardPointer(CFBoard *board);

	/**
	 * @brief Make the move on the current board
	 *
	 * @param move the move to be made
	 */
	void processMove(Move move);

	/**
	 * @brief This function uses the current board to return
	 * a tuple containing the next suggested move and its rating.
	 *
	 *
	 * @return <tuple> where the first element <int> the [0, 63] id of the tile
	 * the piece we're gonna move is located at CURRENTLY, the second element
	 * <int> is the [0, 63] id of the tile we want to move the piece to, and the
	 * third element <float> is the evaluation of the move we're gonna make.
	 */
	virtual Move getNextMove() = 0; // pure virtual function
	CFBoard *currentBoard;
};
}; // namespace Closedfish