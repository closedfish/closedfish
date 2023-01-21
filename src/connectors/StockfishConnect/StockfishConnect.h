#pragma once
#include <EngineWrapper.h>
#include <logger.h>
#include <stockfish/src/bitboard.h>
#include <stockfish/src/endgame.h>
#include <stockfish/src/position.h>
#include <stockfish/src/psqt.h>
#include <stockfish/src/search.h>
#include <stockfish/src/syzygy/tbprobe.h>
#include <stockfish/src/thread.h>
#include <stockfish/src/tt.h>
#include <stockfish/src/uci.h>
#include <tuple>

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