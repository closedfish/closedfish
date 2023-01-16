#include "StockfishConnect.h"

/**
 * @brief converts closedfish board to stockfish board
 *
 * @param cfb closedfish board
 * @param pos stockfish board
 * @param states stockfish statelist pointer
 */
void convert_CFBoard_to_Stockfish_Position(CFBoard cfb,
																					 Stockfish::Position &pos,
																					 Stockfish::StateListPtr &states) {
	std::string fen = cfb.toFEN();
	states = Stockfish::StateListPtr(new std::deque<Stockfish::StateInfo>(1));
	pos.set(fen, false, &states->back(), Stockfish::Threads.main());
}

// look at uci.cpp for reference
std::string call_stockfish(Stockfish::Position &pos,
													 Stockfish::StateListPtr &states,
													 Stockfish::Search::LimitsType limits,
													 bool ponderMode, Closedfish::Logger *logger) {
	limits.time[Stockfish::WHITE] = limits.time[Stockfish::BLACK] =
			Stockfish::TimePoint(1000);
	limits.startTime = Stockfish::now();
	Stockfish::Threads.start_thinking(pos, states, limits, ponderMode);
	std::string line;
	// wait for stockfish threads to finish
	std::this_thread::sleep_until(std::chrono::system_clock::now() +
																std::chrono::seconds(1));
	while (getline(logger->stream, line)) {
		std::stringstream lstream(line);
		std::string cmd;
		lstream >> cmd;
		if (cmd == "bestmove") {
			std::string bestmove;
			lstream >> bestmove;
			return bestmove;
		}
	}
	return "bestmove not found";
}

NextMove StockfishEngine::getNextMove() {
	Stockfish::Position pos;
	Stockfish::StateListPtr states;
	convert_CFBoard_to_Stockfish_Position(*currentBoard, pos, states);
	std::string out = call_stockfish(pos, states, {}, false, logger);
	Stockfish::Threads.set(0);
	return {0, 0, 0.0}; // todo: parse from out
}