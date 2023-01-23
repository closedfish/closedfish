#include "StockfishConnect.h"

// look at uci.cpp for reference
std::string call_stockfish(Stockfish::Position &pos,
													 Stockfish::StateListPtr &states,
													 Stockfish::Search::LimitsType limits,
													 bool ponderMode, Closedfish::Logger *logger) {
	auto &cout = logger ? logger->cout : std::cerr;
	cout << "Calling Stockfish" << std::endl;
	limits.time[Stockfish::WHITE] = limits.time[Stockfish::BLACK] =
			Stockfish::TimePoint(1700);
	limits.startTime = Stockfish::now();
	Stockfish::Threads.start_thinking(pos, states, limits, ponderMode);
	std::string line;
	// wait for stockfish threads to finish
	std::this_thread::sleep_until(std::chrono::system_clock::now() +
																std::chrono::seconds(2));
	Stockfish::Threads.stop = true;
	if (!logger) {
		cout << "Done" << std::endl;
		return "";
	}
	while (getline(logger->stream, line)) {
		std::stringstream lstream(line);
		std::string cmd;
		lstream >> cmd;
		cout << "[LINE] " << line << std::endl;
		if (cmd == "bestmove") {
			std::string bestmove;
			lstream >> bestmove;
			std::string garbage;
			while (lstream >> garbage)
				;
			return bestmove;
		}
	}
	cout << "bestmove not found" << std::endl;
	return "bestmove not found";
}

Closedfish::Move StockfishEngine::getNextMove() {
	Stockfish::Position pos;
	Stockfish::StateListPtr states;
	convert_CFBoard_to_Stockfish_Position(*currentBoard, pos, states);
	std::string out = call_stockfish(pos, states, {}, false, logger);
	Stockfish::Threads.set(size_t(Stockfish::Options["Threads"]));
	if (out.size() != 4) {
		throw "Stockfish invalid output";
	}
	return {parseAN(out.substr(0, 2)), parseAN(out.substr(2, 2)),
					0.0}; // todo: parse from out
}