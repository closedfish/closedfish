#include <iostream>
#include <tuple> 
#include "../board_implementation/CFBoard.h"

class Algorithm {
public:

	/**
	* @brief Abstract constructor, shouldn't be run directly 
	*/
	Algorithm(void) {};


	/**
	* @brief Abstract destructor, shouldn't be run directly
	*/
	~Algorithm(void) {};


	/**
	* @brief Sets the pointer to the current board instance (necessary)
	*
	* @param <CFBoard*> : A pointer to the current board instance. 
	*
	*/
	virtual void setBoardPointer(CFBoard*) {};

	/**
	* @brief This function uses the current board to return 
	* a tuple containing the next suggested move and its rating.
	*
	*
	* @return <tuple> where the first element <int> the [0, 63] id of the tile 
	* the piece we're gonna move is located at CURRENTLY, the second element <int>
	* is the [0, 63] id of the tile we want to move the piece to, and the third element <float>
	* is the evaluation of the move we're gonna make.
	*/
	virtual std::tuple<int, int, float> getNextMove() {};



private:


	CFBoard* currentBoard;
};