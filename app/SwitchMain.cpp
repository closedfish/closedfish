// #include "config.hpp"
#include "SwitchMain.h"
#ifdef _WIN32
#include <direct.h>
#define GETCWD _getcwd
#else
#include <unistd.h>
#define GETCWD getcwd
#endif

// https://stackoverflow.com/questions/11826554/standard-no-op-output-stream/11826666#11826666
class NullBuffer : public std::streambuf {
public:
	int overflow(int c) { return c; }
};
NullBuffer null_buffer;
std::ostream null_stream(&null_buffer);

#define DEBUG
#define MODE_CLI 1

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

/**
 * @brief Convert int on 64 to algebraic notation
 *
 * @param pos postition
 * @return std::string algebraic notation of the position
 */
std::string toAN(int pos) {
	int row = pos >> 3;
	int col = pos & 7;
	return std::string({(char)col + 'a', 7 - (char)row + '1'});
}

/**
 * @brief Convert algebraic notation to int on 64
 *
 * @param str algebraic notation
 * @return int position (8 * row + col)
 */
int parseAN_(std::string str) {
	int row = 7 - (str[1] - '1');
	int col = str[0] - 'a';
	return row * 8 + col;
}
// @TODO: refactor to remove code duplication with `SwitchEngine.cpp`

void CLIGameLoop(SwitchEngine &engine) {
	while (true) {
		debug << "[DEBUG] CLIGameLoop" << std::endl;
		try {
			Closedfish::Move nm = engine.getNextMove();
			debug << toAN(std::get<0>(nm)) << toAN(std::get<1>(nm)) << std::endl;
			engine.processMove(nm);
		} catch (std::string st) {
			debug << "[ERROR] " << st << std::endl;
			exit(1);
		}
		debug << "[INFO] Waiting for opponent\'s move" << std::endl;
		std::string opponentMove;
		std::cin >> opponentMove;
		// quick sanity check
		if (opponentMove.size() == 4 && 'a' <= opponentMove[0] &&
				opponentMove[0] <= 'h' && 'a' <= opponentMove[2] &&
				opponentMove[2] <= 'h' && '1' <= opponentMove[1] &&
				opponentMove[1] <= '8' && '1' <= opponentMove[3] &&
				opponentMove[3] <= '8') {
			Closedfish::Move opp =
					std::make_tuple(parseAN_(opponentMove.substr(0, 2)),
													parseAN_(opponentMove.substr(2, 2)), 0.0);
			debug << "[DEBUG] " << std::get<0>(opp) << " " << std::get<1>(opp)
						<< std::endl;
			engine.processMove(opp);
		} else {
			debug << "[DEBUG] Invalid move string!, got [" << opponentMove
						<< "]: " << opponentMove.size() << std::endl;
			throw "Invalid move string!";
		}
	}
}

typedef void (*OnChange)(const Stockfish::UCI::Option &);

void opt(const Stockfish::UCI::Option &o) { Stockfish::Eval::NNUE::init(); };

int main(int argc, char *argv[]) {
	Closedfish::Logger logger;
	debug << Stockfish::engine_info() << std::endl;
	char buff[40000];
	char *cwd = GETCWD(buff, 40000);
	std::filesystem::path p = std::filesystem::path(CMAKE_SOURCE_DIR) /
														"external" / "stockfish" / "src" /
														"nn-ad9b42354671.nnue";
	/*
	std::ifstream stream(Stockfish::Options["EvalFile"], std::ios::binary);
	if (stream.fail()) {
		debug << "EvalFile doesn't exist. Check NNUE EvalFile." << std::endl;
		exit(-1);
	}
	stream.close();
	*/
	Stockfish::CommandLine::init(argc, argv);
	Stockfish::UCI::init(Stockfish::Options);
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

	ClosedfishEngine a;
	CFBoard board = CFBoard("rkqrbnnb/8/p5p1/Pp1p1pPp/1PpPpP1P/2P1P1BQ/R7/1B1K2RN b - - 0 1");
	a.setBoardPointer(&board);
	std::cerr << board.getReprLegalMove(7, 0) << '\n';
	for (int _ = 0; _ < 15; _++) {
		Closedfish::Move m = a.getNextMove();
		debug << "Move from :" << std::get<0>(m) << " to " << std::get<1>(m) << " with gradient dist: " << std::get<2>(m) << '\n';
		board.movePiece(std::get<0>(m), std::get<1>(m));
		board.forceFlipTurn();
	}

	debug << "[INFO] a.testDFS() done" << std::endl;
	return 0;

	// CFBoard board;
	SwitchEngine engine(board, &logger);

	srand(time(NULL));

	debug << "[INFO] Setup done" << std::endl;

	if (MODE_CLI)
		CLIGameLoop(engine);
	else
		chessGameLoop(engine);

	return 0;
}