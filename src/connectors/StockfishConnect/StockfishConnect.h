#pragma once
#include <EngineWrapper.h>
#include <tuple>
#include <utils.h>

class StockfishEngine : public Closedfish::ChessEngine {
public:
	StockfishEngine() : ChessEngine() {}
	/**
	 * @brief Construct a new Stockfish Engine object
	 *
	 * @param logger pointer to the logger object. The logger should be
	 * initialized in advance.
	 */
	StockfishEngine(Closedfish::Logger *logger) : logger(logger), ChessEngine() {}
	Closedfish::Move getNextMove();

private:
	Closedfish::Logger *logger;
};