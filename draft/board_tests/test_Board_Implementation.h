//Board_Implementation Testing
#pragma once
#include "../../lib/board_implementation/CFBoard.h"
#include <string>
#include <tuple>
#include <vector>

namespace CFBoardTester {
char test_pieceIdToChar(int pieceId);
int test_pieceCharToId(char pieceChar);
uint64_t test_getColorBitBoard(bool color);
uint64_t test_getPieceColorBitBoard(int pieceId);
uint64_t test_getPieceBoardFromIndex(int boardIndex);
...
}
