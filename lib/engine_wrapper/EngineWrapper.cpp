#include "EngineWrapper.h"
void Closedfish::ChessEngine::setBoardPointer(CFBoard *board) {
	currentBoard = board;
}

void Closedfish::ChessEngine::processMove(Closedfish::Move move) {
	if (!currentBoard) {
		throw "Board not found";
	}
	currentBoard->movePiece(std::get<0>(move), std::get<1>(move));
}