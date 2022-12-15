//Board_Implementation Testing
#pragma once
#include "../../lib/board_implementation/CFBoard.h"
#include <string>
#include <tuple>
#include <vector>

namespace CFBoardTester {
    void test_pieceIdToChar();
    void test_pieceCharToId();
    void test_getColorBitBoard();
    void test_getPieceColorBitBoard();
    void test_getPieceBoardFromIndex();
    void test_getCurrentPlayer();
    void test_getPieceFromCoords();
    void test_getBit();
    void test_getMaterialCount();
    void test_addPiece();
    void test_removePiece();
    void test_movePiece();
    void test_forceUndo();
    void test_getCardinals();
    void test_getDiagonals();
    void test_getKnightPattern();
    void test_getKingPattern();
    void test_getPawnPattern();
    void test_getLegalMoves();
    void test_naiveCheckCheck();
    void test_fromFEN();
    void test_toFEN();

}
