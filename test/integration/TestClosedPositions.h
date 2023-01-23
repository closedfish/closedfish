#pragma once

#include <CFBoard.h>
#include <GeneralRegression.h>
#include <SwitchEngine.h>
#include <logger.h>

#include <filesystem>
#include <fstream>
#include <random>
#include <tuple>
#include <vector>

#include <catch2/catch_test_macros.hpp>

static Closedfish::Logger *logger;

class TestClosedPositions {
public:
	static bool run(SwitchEngine engine) {
		while (true) {
			if (engine.currentBoard->naiveCheckCheck(false))
				return true;
			engine.processMove(engine.getNextMove()); // our lib
			if (engine.currentBoard->naiveCheckCheck(true))
				return true;
			engine.processMove(engine.getStockfishMove()); // their lib
		}
		return false;
	}
	typedef void (*OnChange)(const Stockfish::UCI::Option &);
	static void opt(const Stockfish::UCI::Option &o) {
		Stockfish::Eval::NNUE::init();
	};
	static void init() {
		Closedfish::Logger *currentLogger = new Closedfish::Logger();
		char *argv[1];
		argv[0] = "";
		Stockfish::CommandLine::init(1, argv);
		Stockfish::UCI::init(Stockfish::Options);
		std::filesystem::path p = std::filesystem::path(CMAKE_SOURCE_DIR) /
															"external" / "stockfish" / "src" /
															(std::string)Stockfish::Options["EvalFile"];
		std::ifstream stream(p.string(), std::ios::binary);
		if (stream.fail()) {
			std::cerr << "[ERROR] EvalFile doesn't exist. Expected directory: "
								<< p.string() << std::endl;
			exit(-1);
		}
		stream.close();
		Stockfish::Options["EvalFile"]
				<< Stockfish::UCI::Option(p.string().c_str(), opt);
		Stockfish::Tune::init();
		Stockfish::PSQT::init();
		Stockfish::Bitboards::init();
		Stockfish::Position::init();
		Stockfish::Bitbases::init();
		Stockfish::Endgames::init();
		Stockfish::Threads.set(size_t(Stockfish::Options["Threads"]));
		Stockfish::Search::clear(); // After threads are up
		Stockfish::Eval::NNUE::init();
		Stockfish::Position::init();
		EvaluationFunction::init();
		srand(time(NULL));
		logger = currentLogger;
	}
};