/**
 * try_interface.cpp -- Try importing stockfish "directly"
 *
 */

#include "../Board Implementation/CFBoard.cpp"
#include "Stockfish/src/uci.cpp"
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
    states = Stockfish::StateListPtr(new std::deque<StateInfo>(1));
    pos.set(fen, false, &states->back(), Stockfish::Threads.main());
}

// look at uci.cpp for reference
void call_stockfish(Stockfish::Position &pos, Stockfish::StateListPtr &states,
                    Search::LimitsType limits, bool ponderMode) {
    limits.startTime = now();
    Threads.start_thinking(pos, states, limits, ponderMode);
}
