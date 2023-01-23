#pragma once
#include <CFBoard.h>
#include <ClosedfishConnect.h>
#include <EngineWrapper.h>
#include <StockfishConnect.h>
#include <tuple>
#include <utils.h>

class SwitchEngine : public Closedfish::ChessEngine {
public:
	enum Status { CLOSED, OPEN };
	/**
	 * @brief Construct a new Switch Engine object
	 *
	 * @param board
	 * @param logger
	 */
	SwitchEngine() : ChessEngine(), status(Status::OPEN) {}
	SwitchEngine(CFBoard &board, Closedfish::Logger *logger);
	Closedfish::Move getNextMove();
	Closedfish::Logger
			*logger; // should be accessed publicly as a substitute for std::cout.
	Closedfish::Move getStockfishMove();

private:
	ClosedfishEngine *closedfish;
	StockfishEngine *stockfish;
	Status status;
};