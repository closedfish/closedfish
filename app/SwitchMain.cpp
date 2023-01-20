// #include "config.hpp"
#include "SwitchMain.h"
#ifdef _WIN32
#include <direct.h>
#define GETCWD _getcwd
#else
#include <unistd.h>
#define GETCWD getcwd
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
		std::cerr << "[DEBUG] CLIGameLoop" << std::endl;
		try {
			Closedfish::Move nm = engine.getNextMove();
			std::cerr << toAN(std::get<0>(nm)) << toAN(std::get<1>(nm)) << std::endl;
			engine.processMove(nm);
		} catch (std::string st) {
			std::cerr << "[ERROR] " << st << std::endl;
			exit(1);
		}
		std::cerr << "[INFO] Waiting for opponent\'s move" << std::endl;
		std::string opponentMove;
		std::cin >> opponentMove;
		// quick sanity check
		if (opponentMove.size() == 4 && 'a' <= opponentMove[0] &&
				opponentMove[0] <= 'h' && 'a' <= opponentMove[2] &&
				opponentMove[2] <= 'h' && '0' <= opponentMove[1] &&
				opponentMove[1] < '8' && '0' <= opponentMove[3] &&
				opponentMove[3] < '8') {
			Closedfish::Move opp =
					std::make_tuple(parseAN_(opponentMove.substr(0, 2)),
													parseAN_(opponentMove.substr(2, 2)), 0.0);
			engine.processMove(opp);
		} else {
			throw "Invalid move string!";
		}
	}
}

typedef void (*OnChange)(const Stockfish::UCI::Option &);

void opt(const Stockfish::UCI::Option &o) { Stockfish::Eval::NNUE::init(); };

int main(int argc, char *argv[]) {
	bool useCLI = argc >= 2 && std::string(argv[1]) ==
																 std::string("cli"); // use CLI instead of GUI
	Closedfish::Logger logger;
	std::cerr << Stockfish::engine_info() << std::endl;
	char buff[40000];
	char *cwd = GETCWD(buff, 40000);
	std::filesystem::path p = std::filesystem::path(cwd) / "external" /
														"stockfish" / "src" / "nn-ad9b42354671.nnue";
	/*
	std::ifstream stream(Stockfish::Options["EvalFile"], std::ios::binary);
	if (stream.fail()) {
		std::cerr << "EvalFile doesn't exist. Check NNUE EvalFile." << std::endl;
		exit(-1);
	}
	stream.close();
	*/
	Stockfish::CommandLine::init(1, argv);
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

	CFBoard board;
	SwitchEngine engine(board, &logger);

	srand(time(NULL));

	std::cerr << "[INFO] Setup done" << std::endl;

	if (useCLI)
		CLIGameLoop(engine);
	else
		chessGameLoop(engine);
	return 0;
}