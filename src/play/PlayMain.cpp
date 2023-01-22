#include "PlayMain.h"
#include "GeneralRegression.h"
#include <cmath>
#include <ctype.h>
#include <iostream>

std::pair<std::vector<int>, std::vector<int>> convert_board(std::string fen) {
	int sz = 0;
	while (sz < fen.size() && fen[sz] != ' ') {
		sz += 1;
	}

	std::string sub_fen = fen.substr(0, sz);

	std::vector<int> L_top(8), L_bottom(8);
	std::fill(L_top.begin(), L_top.end(), 8);
	std::fill(L_bottom.begin(), L_bottom.end(), -1);

	int counter = 0;
	int row = 0;

	for (int j = 0; j < sz; j++) {
		char chess_piece = fen[j];

		if (chess_piece == '/') {
			counter = 0;
			row += 1;
		}

		else if (isdigit(chess_piece)) {
			counter += (int)chess_piece - '0';
		}

		else {
			if (chess_piece == 'p') {
				L_bottom[counter] = row;
			}

			if ((chess_piece == 'P') && (L_top[counter] == 8)) {
				L_top[counter] = row;
			}

			counter += 1;
		}
	}

	return std::make_pair(L_top, L_bottom);
}

float evaluateOpenness(std::string fen) {
	std::pair<std::vector<int>, std::vector<int>> board = convert_board(fen);
	return EvaluationFunction::Evaluate(board.first, board.second, 23);
}

/*
int main() {
	EvaluationFunction::init();
	std::cout << evaluateOpenness(
			"rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP3K1/1P6/RNBQ1B1R w - - 0 1");
	return 0;
}
*/
