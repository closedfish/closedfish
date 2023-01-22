#include "../../lib/board_implementation/CFBoard.h"
#include <iostream>
#include <tuple>
#include <vector>

/**
 * @brief Lightweight unit test
 *
 * @param testcases
 */
void test(
		std::vector<std::tuple<std::string, int, int, std::string>> testcases) {
	for (auto testcase : testcases) {
		std::string fen;
		int a, b;
		std::string expectedRepr;
		tie(fen, a, b, expectedRepr) = testcase;
		CFBoard board(fen);
		std::string outcome = board.getReprLegalMove(a, b);
		if (outcome == expectedRepr) {
			std::cout << "[SUCCESS] on input " << fen << " (" << a << ", " << b
								<< "); with output\n"
								<< expectedRepr << std::endl;
		} else {
			std::cout << "[WRONG] on input " << fen << " (" << a << ", " << b
								<< "); expectedRepr\n"
								<< expectedRepr << "retrieved\n"
								<< outcome << std::endl;
		}
	}
}

int main() {
	std::cout << "Quick test: "
						<< (int)CFBoard(
									 "2k5/p7/qp5p/1qp3pP/B1qp1pP1/3npPN1/P3PB1R/3KQ2R w - - 0 1")
									 .naiveCheckCheck(false, 59, 50)
						<< std::endl;
	std::cout << "Quick test: "
						<< (int)CFBoard(
									 "2k5/p7/qp5p/1qp3pP/B1qp1pP1/3npPN1/P1K1PB1R/4Q2R b - - 0 1")
									 .naiveCheckCheck(false)
						<< std::endl;

	test({{"2k5/p7/qp5p/1qp3pP/B1qp1pP1/3npPN1/P3PB1R/3KQ2R w - - 0 1", 10, 59,
				 "| . | . | k | . | . | . | . | . |\n"
				 "| p | . | . | . | . | . | . | . |\n"
				 "| q | p | . | . | . | . | . | p |\n"
				 "| . | q | p | . | . | . | p | P |\n"
				 "| B | . | q | p | . | p | P | . |\n"
				 "| . | . | . | n | p | P | N | . |\n"
				 "| P | . | . | . | P | B | . | R |\n"
				 "| . | . | . |>K<| Q | . | . | R |\n|"}});
}