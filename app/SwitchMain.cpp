#include "SwitchMain.h"

// https://stackoverflow.com/questions/11826554/standard-no-op-output-stream/11826666#11826666
class NullBuffer : public std::streambuf {
public:
	int overflow(int c) { return c; }
};
NullBuffer null_buffer;
std::ostream null_stream(&null_buffer);

#ifdef DEBUG
#define debug std::cerr
#else
#define debug null_stream
#endif

/*
Closedfish::Move InputFromUI() { return {0, 0, 0.0}; }

void chessGameLoop(SwitchEngine &engine) {
	while (true) {
		Closedfish::Move nm = engine.getNextMove();
		// now process this object
		OutputToUI();
		engine.processMove(nm);
		// get opponent's next move
		Closedfish::Move opp = InputFromUI();
		engine.processMove(opp);
	}
}

std::vector<Closedfish::Move> moves(
		{{51, 35, 0.0}, {6, 21, 0.0},	 {50, 34, 0.0}, {12, 20, 0.0}, {57, 42, 0.0},
		 {5, 33, 0.0},	{62, 45, 0.0}, {21, 36, 0.0}, {59, 50, 0.0}, {13, 29, 0.0},
		 {54, 46, 0.0}, {9, 17, 0.0},	 {61, 54, 0.0}, {2, 9, 0.0},	 {45, 51, 0.0},
		 {33, 42, 0.0}, {49, 42, 0.0}, {36, 19, 0.0}, {54, 9, 0.0},	 {19, 9, 0.0},
		 {52, 36, 0.0}, {4, 6, 0.0},	 {60, 62, 0.0}, {1, 18, 0.0},	 {58, 40, 0.0},
		 {18, 12, 0.0}, {56, 60, 0.0}, {10, 26, 0.0}, {53, 37, 0.0}, {5, 4, 0.0},
		 {35, 27, 0.0}, {9, 19, 0.0},	 {36, 28, 0.0}, {19, 13, 0.0}, {51, 45, 0.0},
		 {20, 27, 0.0}, {34, 27, 0.0}, {12, 27, 0.0}, {50, 29, 0.0}, {27, 42, 0.0},
		 {29, 38, 0.0}, {13, 23, 0.0}, {38, 31, 0.0}, {42, 27, 0.0}, {37, 29, 0.0},
		 {11, 19, 0.0}, {29, 21, 0.0}, {4, 28, 0.0},	{45, 28, 0.0}, {27, 21, 0.0},
		 {31, 23, 0.0}, {14, 23, 0.0}, {28, 18, 0.0}, {3, 5, 0.0},	 {18, 12, 0.0},
		 {6, 14, 0.0},	{40, 49, 0.0}, {0, 4, 0.0},		{60, 20, 0.0}});

void debugGameLoop(SwitchEngine &engine) {
	for (int i = 0; i < moves.size(); i++) {
		debug << engine.currentBoard->getRepr() << std::endl;
		engine.processMove(moves[i]);
	}
	debug << "[ALL GOOD]" << std::endl;
}
*/

void CLIGameLoop(SwitchEngine &engine) {
	while (true) {
		debug << "[DEBUG] CLIGameLoop" << std::endl;
		std::string opponentMove;
		std::cin >> opponentMove;
		// quick sanity check
		if (opponentMove.size() == 4 && 'a' <= opponentMove[0] &&
				opponentMove[0] <= 'h' && 'a' <= opponentMove[2] &&
				opponentMove[2] <= 'h' && '1' <= opponentMove[1] &&
				opponentMove[1] <= '8' && '1' <= opponentMove[3] &&
				opponentMove[3] <= '8') {
			Closedfish::Move opp =
					std::make_tuple(parseAN(opponentMove.substr(0, 2)),
													parseAN(opponentMove.substr(2, 2)), 0.0);
			debug << "[DEBUG] " << std::get<0>(opp) << " " << std::get<1>(opp)
						<< std::endl;
			engine.processMove(opp);
		} else {
			debug << "[DEBUG] Invalid move string!, got [" << opponentMove
						<< "]: " << opponentMove.size() << std::endl;
			throw "Invalid move string!";
		}
		try {
			Closedfish::Move nm = engine.getNextMove();
			debug << toAN(std::get<0>(nm)) << toAN(std::get<1>(nm)) << std::endl;
			engine.processMove(nm);
		} catch (std::string st) {
			debug << "[ERROR] " << st << std::endl;
			exit(1);
		}
		debug << "[INFO] Waiting for opponent\'s move" << std::endl;
	}
}

typedef void (*OnChange)(const Stockfish::UCI::Option &);

void opt(const Stockfish::UCI::Option &o) { Stockfish::Eval::NNUE::init(); };

int main(int argc, char *argv[]) {
	Closedfish::Logger logger;
	debug << "[INFO] " << Stockfish::engine_info() << std::endl;
	Stockfish::CommandLine::init(argc, argv);
	Stockfish::UCI::init(Stockfish::Options);
	std::filesystem::path p = std::filesystem::path(CMAKE_SOURCE_DIR) /
														"external" / "stockfish" / "src" /
														(std::string)Stockfish::Options["EvalFile"];
	std::ifstream stream(p.string(), std::ios::binary);
	if (stream.fail()) {
		debug << "[ERROR] EvalFile doesn't exist. Expected directory: "
					<< p.string() << std::endl;
		exit(-1);
	} else {
		debug << "[INFO] EvalFile found." << std::endl;
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

	CFBoard board;
	SwitchEngine engine(board, &logger);

	srand(time(NULL));

	debug << "[INFO] Setup done" << std::endl;
	CLIGameLoop(engine);

	return 0;
}