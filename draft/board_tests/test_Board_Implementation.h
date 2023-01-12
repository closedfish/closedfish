//Board_Implementation Testing
#pragma once
#include "../../lib/board_implementation/CFBoard.h"
#include <string>
#include <tuple>
#include <vector>

namespace CFBoardTester {
    void test_pieceIdToChar(); // Etienne - done
    // tests each piece, pieceId from 0-11, and compares pieceIdtoChar output to expected output

    void test_pieceCharToId(); // Shirelle - done
    // tests each piece, pieceChar for P/N/B/R/Q/K, and compares pieceChartoId output to expected output

    void test_getColorBitBoard(); //Etienne - done - Check - could be too basic
    // tests the default board, checks if white and black BitBoard match

    void test_getPieceColorBitBoard(); // Shirelle
    //

    void test_getPieceBoardFromIndex(); // Etienne - done
    //

    void test_getCurrentPlayer(); //Etienne

    void test_getPieceFromCoords(); // Etienne - done
    void test_getBit(); // Etienne - done
    void test_getMaterialCount(); // Etienne - done
    void test_addPiece(); // Etienne 
    void test_removePiece();
    void test_movePiece();
    void test_forceUndo();
    void test_getCardinals();
    void test_getDiagonals();
    void test_getKnightPattern(); // shirelle
    void test_getKingPattern();// shirelle
    void test_getPawnPattern();
    void test_getLegalMoves();
    void test_naiveCheckCheck();
    void test_fromFEN();
    void test_toFEN();

}
