#include "utils.h"

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

/**
 * @brief Convert int on 64 to algebraic notation
 *
 * @param pos postition
 * @return std::string algebraic notation of the position
 */
std::string toAN(int pos) {
	int row = pos >> 3;
	int col = pos & 7;
	return std::string(
			{static_cast<char>(col + 'a'), static_cast<char>(7 - row + '1')});
}

/**
 * @brief Convert algebraic notation to int on 64
 *
 * @param str algebraic notation
 * @return int position (8 * row + col)
 */
int parseAN(std::string str) {
	int row = 7 - (str[1] - '1');
	int col = str[0] - 'a';
	return row * 8 + col;
}