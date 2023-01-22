#pragma once
#include <string>

/**
 * Main switch openness function.
 * Gives the openness evaluation (between 0.0: closed to 1.0: open)
 *
 * @param fen board in FEN format
 */
float evaluateOpenness(std::string fen);