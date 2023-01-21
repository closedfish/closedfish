#include "ClosedfishConnect.h"

Closedfish::Move ClosedfishEngine::getNextMove() {
	// to be completed by @algo.
	DFS1P d;
	d.setBoardPointer(currentBoard);
	Closedfish::Move nextMoveDFS1P = d.getNextMove();
	if (std::get<2>(nextMoveDFS1P) <= 0) { // The move doesn't help up get closer to the heatmap
		// Call Aarrya's function
		return std::make_tuple(0, 0, 0);
	} else {
		return nextMoveDFS1P;
	}
}