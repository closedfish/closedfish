#include "../../lib/board_implementation/CFBoard.h"
#include <iostream>
#include <tuple>
#include <vector>

/**
 * @brief Lightweight unit test
 *
 * @param testcases
 */
void test(std::vector<std::tuple<std::string, int, int, int, bool>> testcases) {
  for (auto testcase : testcases) {
    std::string fen;
    int a, b, c;
    bool expected;
    tie(fen, a, b, c, expected) = testcase;
    CFBoard board(fen);
    bool outcome = board.naiveCheckCheck(a, b, c);
    if (outcome == expected) {
      std::cout << "[SUCCESS] on input " << fen << " (" << a << ", " << b
                << ", " << c << "); with output " << expected << std::endl;
    } else {
      std::cout << "[WRONG] on input " << fen << " (" << a << ", " << b << ", "
                << c << "); expected " << expected << " retrieved " << outcome
                << std::endl;
    }
  }
}

int main() {
  test({{"rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP3K1/1P6/RNBQ1B1R w - - 0 1",
         0, 38, 55, false},
        {"rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP1K2/PpP1N3/1P6/RNBQ1BR1 w - - 0 1",
         0, -1, -1, false}});
}