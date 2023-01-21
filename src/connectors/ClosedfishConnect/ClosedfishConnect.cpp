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
		Closedfish::Move breakthrough = b.getNextMove();
		if (std::get<0>(breakthrough) == 0 && std::get<1>(breakthrough) == 0) {
			doDFS1PFlag = 1;
			DFS1P d;
			d.setBoardPointer(currentBoard);
			lastMove = d.getNextMove();
			return lastMove;
		}
		lastMove = b.getNextMove();
		std::cerr << std::get<0>(lastMove) << ' ' << std::get<1>(lastMove) << "BREAKTHROUGH\n";
	}
	return lastMove;
}