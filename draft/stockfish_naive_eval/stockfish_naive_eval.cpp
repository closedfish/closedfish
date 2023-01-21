/**
 * stockfish_naive_eval.cpp -- Utility for evaluating the board using
 * Stockfish's direct evaluation
 */

#include "stockfish_naive_eval.h"
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

Stockfish::Value stockfishNaiveEval(CFBoard board) {
    Stockfish::Position pos;
    Stockfish::StateListPtr states;
    convert_CFBoard_to_Stockfish_Position(board, pos, states);
    return Stockfish::Eval::evaluate(pos);
}