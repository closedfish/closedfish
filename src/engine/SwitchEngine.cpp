#include "SwitchEngine.h"

SwitchEngine::SwitchEngine(CFBoard &board, Closedfish::Logger &logger)
		: ChessEngine() {
	ChessEngine::setBoardPointer(&board);
	closedfish = new ClosedfishEngine();
	closedfish->setBoardPointer(&board);
	stockfish = new StockfishEngine(logger);
	stockfish->setBoardPointer(&board);
}

NextMove SwitchEngine::getNextMove() { return {0, 0, 0.0}; }