//Board_Implementation Testing
#pragma once
#include "../../lib/board_implementation/CFBoard.h"
#include <catch2/catch_test_macros.hpp>
#include <string>
#include <tuple>
#include <vector>

namespace CFBoardTester {
    void test_pieceIdToChar(); // done
    // tests each piece, pieceId from 0-11, and compares pieceIdtoChar output to expected output

    void test_pieceCharToId(); // done
    // tests each piece, pieceChar for P/N/B/R/Q/K, and compares pieceChartoId output to expected output

    void test_getColorBitBoard(); //done - Check - could be too basic
    // tests the default board, checks if white and black BitBoard match

    void test_getPieceColorBitBoard(); //
    //

    void test_getPieceBoardFromIndex(); //done
    //tests the default board, checks every board from pawn - king

    void test_getCurrentPlayer(); //done - could be too basic
    //tests the default board, tests that white is the first move.

    void test_getPieceFromCoords(); // done

    void test_getBit(); // done

    void test_getMaterialCount(); // done

    void test_addPiece();
    void test_removePiece();
    void test_movePiece();
    void test_forceUndo();
    void test_getCardinals();
    void test_getDiagonals();

    void test_getKnightPattern(); // done

    void test_getKingPattern();// done

    void test_getPawnPattern(); // done

    void test_getLegalMoves();

    void test_naiveCheckCheck(); // done

    void test_fromFEN(); // done

    void test_toFEN(); // done

}
