//Board_Implementation Testing
#pragma once
#include "../../lib/board_implementation/CFBoard.h"
#include <string>
#include <tuple>
#include <vector>

namespace CFBoardTester {
    void test_pieceIdToChar(); // Etienne - done
    void test_pieceCharToId(); // Shirelle - done
    void test_getColorBitBoard(); //Etienne
    void test_getPieceColorBitBoard(); // Shirelle
    void test_getPieceBoardFromIndex(); // Etienne
    void test_getCurrentPlayer(); // Shirelle
    void test_getPieceFromCoords(); // Etienne - done
    void test_getBit(); // Etienne - done
    void test_getMaterialCount(); // Etienne - done
    void test_addPiece(); // Etienne 
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
