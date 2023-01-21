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

int moveCount;
void CLIGameLoop(SwitchEngine &engine) {
	std::vector<Closedfish::Move> moves = {
		{parseAN("e2"), parseAN("e4"), 0.0},
		{parseAN("d7"), parseAN("d5"), 0.0},
		{parseAN("e4"), parseAN("d5"), 0.0},
		{parseAN("d8"), parseAN("d5"), 0.0},
		{parseAN("b1"), parseAN("c3"), 0.0},
		{parseAN("d5"), parseAN("e6"), 0.0},
		{parseAN("f1"), parseAN("e2"), 0.0}
	};
	moveCount = 0;
	while (moveCount < moves.size()) {
		debug << "[DEBUG] CLIGameLoop" << std::endl;
		debug << "[INFO] getReprLegalMove\n"
						<< engine.currentBoard->getRepr()
						<< std::endl;
		debug << "[INFO] Waiting for opponent\'s move" << std::endl;
		// std::string opponentMove;
		// std::cin >> opponentMove;
		// if (opponentMove.size() == 4 && 'a' <= opponentMove[0] &&
		// 		opponentMove[0] <= 'h' && 'a' <= opponentMove[2] &&
		// 		opponentMove[2] <= 'h' && '1' <= opponentMove[1] &&
		// 		opponentMove[1] <= '8' && '1' <= opponentMove[3] &&
		// 		opponentMove[3] <= '8') {
			Closedfish::Move opp = moves[moveCount++];
					// std::make_tuple(parseAN(opponentMove.substr(0, 2)),
					// 								parseAN(opponentMove.substr(2, 2)), 0.0);
			debug << "[DEBUG] " << std::get<0>(opp) << " " << std::get<1>(opp)
						<< std::endl;
			engine.processMove(opp);
		// } else {
		// 	debug << "[DEBUG] Invalid move string!, got [" << opponentMove
		// 				<< "]: " << opponentMove.size() << std::endl;
		// 	throw "Invalid move string!";
		// }
		try {
			Closedfish::Move nm = moves[moveCount++]; //engine.getNextMove();
			debug << toAN(std::get<0>(nm)) << toAN(std::get<1>(nm)) << std::endl;
			engine.processMove(nm);
		} catch (std::string st) {
			debug << "[ERROR] " << st << std::endl;
			exit(1);
		}
		// quick sanity check
	}
	debug << "[ALL GOOD]" << std::endl;
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

	// DFS1P a;
	// a.testDFS();

	debug << "[INFO] a.testDFS() done" << std::endl;

	CFBoard board;
	SwitchEngine engine(board, &logger);

	srand(time(NULL));

	debug << "[INFO] Setup done" << std::endl;

	if (MODE_CLI)
		CLIGameLoop(engine);
	else
		chessGameLoop(engine);

	return 0;
}