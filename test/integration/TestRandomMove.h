#pragma once

#include <CFBoard.h>
#include <SwitchEngine.h>
#include <logger.h>

#include <filesystem>
#include <fstream>
#include <random>
#include <tuple>
#include <vector>

#include <catch2/catch_test_macros.hpp>

static Closedfish::Logger *logger;

class TestRandomMove {
public:
	static bool run(Closedfish::ChessEngine &engine1,
									Closedfish::ChessEngine &engine2) {
		while (true) {
			if (engine1.currentBoard->naiveCheckCheck(false))
				return true;
			engine1.processMove(engine1.getNextMove());
			if (engine1.currentBoard->naiveCheckCheck(true))
				return true;
			engine1.processMove(engine2.getNextMove());
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
		srand(time(NULL));
		logger = currentLogger;
	}
};

class RandomMoveEngine : public Closedfish::ChessEngine {
private:
	std::default_random_engine dre;

public:
	RandomMoveEngine(CFBoard &board) : ChessEngine() {
		setBoardPointer(&board);
		dre.seed(time(NULL));
	}
	Closedfish::Move getNextMove() {
		std::vector<std::tuple<int, int, float>> nextMoves;
		// edited from Aarrya's code in dfs breakthrough
		for (int sq = 0; sq < 64; sq++) {
			int p = currentBoard->getPieceFromCoords(sq);
			if (p >= 0 && (p & 1) == (currentBoard->getCurrentPlayer() ? 1 : 0)) {
				uint64_t moves = currentBoard->getLegalMoves(p, sq);
				for (int nsq = 0; nsq < 64; nsq++) {
					if (moves & (1ull << (uint64_t)nsq)) {
						nextMoves.push_back({sq, nsq, 0.0});
					}
				}
			}
		}
		shuffle(nextMoves.begin(), nextMoves.end(), dre);
		return nextMoves.back();
	}
};