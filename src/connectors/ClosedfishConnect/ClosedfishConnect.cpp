#include "ClosedfishConnect.h"

Closedfish::Move ClosedfishEngine::getNextMove() {
	// to be completed by @algo.
	if (doDFS1PFlag) {
		DFS1P d;
		d.setBoardPointer(currentBoard);
		Closedfish::Move nextMoveDFS1P = d.getNextMove();
		if (std::get<1>(nextMoveDFS1P) == std::get<0>(lastMove) && 
		std::get<0>(nextMoveDFS1P) == std::get<1>(lastMove)) { // Repeated moves
			doDFS1PFlag = 0;
		}
		std::cerr << "Continue 1PDFS\n";
		lastMove = nextMoveDFS1P;
	}
	else {
		// Call Aarrya's function
		std::cerr << "BREAKTHROUGH\n";
		Breakthrough2 b;
		b.setBoardPointer(currentBoard);
		lastMove = b.getNextMove();
	}
	return lastMove;
}