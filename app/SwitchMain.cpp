// #include "config.hpp"
#include "SwitchMain.h"

int main(int argc, char *argv[]) {
	Closedfish::Logger logger;

	std::cout << Stockfish::engine_info() << std::endl;
	std::cerr << "Hello, world!" << std::endl;
	std::string tst;
	logger.stream >> tst;
	std::cerr << tst << std::endl;
	Stockfish::CommandLine::init(argc, argv);
	Stockfish::UCI::init(Stockfish::Options);
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
	Stockfish::Position pos;
	Stockfish::StateListPtr states;

	CFBoard board;
	srand(time(NULL));

	double ClosenessCoef;
	double BestClosedfishScore, BestStockfishScore;

	BestStockfishScore = ((double)rand() / RAND_MAX) + 1;
	BestClosedfishScore = ((double)rand() / RAND_MAX) + 1;
	ClosenessCoef = ((double)rand() / RAND_MAX) + 1;

	if (ClosenessCoef * BestStockfishScore >
			(1 - ClosenessCoef) * BestClosedfishScore) {
		std::cout << "We choose Closedfish" << std::endl;
	} else {
		std::cout << "We choose Stockfish" << std::endl;
	}

	OutputToUI();

	return 0;
}