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
 * @return Position stockfish board
 */
Position convert_CFBoard_to_Stockfish_Position(CFBoard cfb) {
    // to be implemented
}

// look at uci.cpp for reference
void call_stockfish(Position &pos, StateListPtr &states,
                    Search::LimitsType limits, bool ponderMode) {
    limits.startTime = now();
    Threads.start_thinking(pos, states, limits, ponderMode);
}