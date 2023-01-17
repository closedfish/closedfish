#pragma once
#include <CFBoard.h>
#include <ClosedfishConnect.h>
#include <EngineWrapper.h>
#include <StockfishConnect.h>
#include <logger.h>
#include <tuple>

class SwitchEngine : public Closedfish::ChessEngine {
public:
	enum Status { CLOSED, OPEN };
	/**
	 * @brief Construct a new Switch Engine object
	 *
	 * @param board
	 * @param logger
	 */
	SwitchEngine() : ChessEngine() {}
	SwitchEngine(CFBoard &board, Closedfish::Logger &logger);
	Closedfish::Move getNextMove();

private:
	ClosedfishEngine *closedfish;
	StockfishEngine *stockfish;
	Status status;
};