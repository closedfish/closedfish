#include "SwitchEngine.h"
#include <PlayMain.h>

SwitchEngine::SwitchEngine(CFBoard &board, Closedfish::Logger *logger)
		: ChessEngine(), logger(logger) {
	ChessEngine::setBoardPointer(&board);
	closedfish = new ClosedfishEngine();
	closedfish->setBoardPointer(&board);
	stockfish = new StockfishEngine(logger);
	stockfish->setBoardPointer(&board);
}

Closedfish::Move SwitchEngine::getNextMove() {
	float opennessCoef = evaluateOpenness(currentBoard->toFEN());
	// Proposal: Maybe change this, depending on breakthrough
	if (status == Status::CLOSED && opennessCoef >= 0.25)
		status = Status::OPEN;
	else if (status == Status::OPEN && opennessCoef < 0.25)
		status = Status::CLOSED;
	(logger ? logger->cout : std::cerr) << "DBG " << opennessCoef << std::endl;
	if (status == Status::CLOSED) {
		// We choose Closedfish
		return closedfish->getNextMove();
	} else {
		// We choose Stockfish
		return stockfish->getNextMove();
	}
}