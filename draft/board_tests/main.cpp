#include "test_Board_Implementation.h"

int main() {
    CFBoardTester::test_pieceIdToChar();
    CFBoardTester::test_pieceCharToId();
    CFBoardTester::test_getColorBitBoard();
    CFBoardTester::test_getPieceColorBitBoard();
    CFBoardTester::test_getPieceBoardFromIndex();
    CFBoardTester::test_getCurrentPlayer();
    CFBoardTester::test_getPieceFromCoords();
    CFBoardTester::test_getBit();
    CFBoardTester::test_getMaterialCount();
    CFBoardTester::test_addPiece();
    CFBoardTester::test_removePiece();
    CFBoardTester::test_movePiece();
    CFBoardTester::test_forceUndo();
    CFBoardTester::test_getCardinals();
    CFBoardTester::test_getDiagonals();
    CFBoardTester::test_getKnightPattern();
    CFBoardTester::test_getKingPattern();
    CFBoardTester::test_getPawnPattern();
    CFBoardTester::test_getLegalMoves();
    CFBoardTester::test_naiveCheckCheck();
    CFBoardTester::test_fromFEN();
    CFBoardTester::test_toFEN();

    return 0;
}
