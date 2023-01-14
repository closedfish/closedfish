/**
 * try_interface.cpp -- Try importing stockfish "directly"
 *
 */

#include "../../external/stockfish/src/bitboard.h"
#include "../../external/stockfish/src/endgame.h"
#include "../../external/stockfish/src/position.h"
#include "../../external/stockfish/src/psqt.h"
#include "../../external/stockfish/src/search.h"
#include "../../external/stockfish/src/syzygy/tbprobe.h"
#include "../../external/stockfish/src/thread.h"
#include "../../external/stockfish/src/tt.h"
#include "../../external/stockfish/src/uci.h"
#include "../../lib/board_implementation/CFBoard.h"
#include "logger.h"
#include <cstdio>
#include <iostream>
#include <vector>

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

Closedfish::Logger *logger = nullptr;

// look at uci.cpp for reference
std::string call_stockfish(Stockfish::Position &pos,
                           Stockfish::StateListPtr &states,
                           Stockfish::Search::LimitsType limits,
                           bool ponderMode) {
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

int main(int argc, char *argv[]) {
    logger = new Closedfish::Logger();
    std::cout << Stockfish::engine_info() << std::endl;
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
    Stockfish::Position::init(); // if not yet init (should rather be inside the
                                 // main function)
    Stockfish::Position pos;
    Stockfish::StateListPtr states;
    convert_CFBoard_to_Stockfish_Position(CFBoard(), pos, states);
    std::string out = call_stockfish(pos, states, {}, false);
    Stockfish::Threads.set(0);
    delete logger;
    std::cout << out << std::endl;
}