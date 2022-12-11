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
    Stockfish::Position::init(); // if not yet init (should rather be inside the
                                 // main function)
    std::string fen = cfb.toFEN();
    states = Stockfish::StateListPtr(new std::deque<Stockfish::StateInfo>(1));
    pos.set(fen, false, &states->back(), Stockfish::Threads.main());
}

// look at uci.cpp for reference
void call_stockfish(Stockfish::Position &pos, Stockfish::StateListPtr &states,
                    Stockfish::Search::LimitsType limits, bool ponderMode) {
    Closedfish::Logger logger;
    limits.startTime = Stockfish::now();
    Stockfish::Threads.start_thinking(pos, states, limits, ponderMode);
    std::string line;
    while (getline(logger.stream, line)) {
        std::cerr << "[CERR] " << line << std::endl;
    }
}

int main() {
    Stockfish::Position pos;
    Stockfish::StateListPtr states;
    convert_CFBoard_to_Stockfish_Position(CFBoard(), pos, states);
    call_stockfish(pos, states, {}, false);
}