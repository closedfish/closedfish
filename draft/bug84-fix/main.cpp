#include "../../lib/board_implementation/CFBoard.h"
#include <iostream>

int main() {
  CFBoard board = CFBoard("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP3K1/1P6/"
                          "RNBQ1B1R w - - 0 1"); // wrong naiveCheckCheck
  std::cout << board.naiveCheckCheck(0, 38, 55) << '\n';
}