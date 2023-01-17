#include "SwitchEngine.h"

SwitchEngine::SwitchEngine(CFBoard &board, Closedfish::Logger &logger)
		: ChessEngine() {
	ChessEngine::setBoardPointer(&board);
	closedfish = new ClosedfishEngine();
	closedfish->setBoardPointer(&board);
	stockfish = new StockfishEngine(logger);
	stockfish->setBoardPointer(&board);
}

Closedfish::Move SwitchEngine::getNextMove() {
	double ClosenessCoef;
	ClosenessCoef = ((double)rand() / RAND_MAX) + 1;
	if (status == Status::CLOSED && ClosenessCoef > 0.6)
		status = Status::OPEN;
	else if (status == Status::OPEN && ClosenessCoef < 0.8)
		status = Status::CLOSED;
	if (status == Status::CLOSED) {
		// We choose Closedfish
		return closedfish->getNextMove();
	} else {
		// We choose Stockfish
		return stockfish->getNextMove();
	}
}