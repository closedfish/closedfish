# Simple unit tests for CFBoard

--To compile: `g++ -std=c++11 -O2 main.cpp test_from_fen.cpp test_naive_check_check.cpp test_to_fen.cpp ../../lib/board_implementation/CFBoard.cpp ../../lib/board_implementation/naiveCheckCheck.cpp -o tests`--

Now replaced with Catch2 testing framework.

This tests:
- `test_pieceIdToChar`
- `test_pieceCharToId`
- `test_getColorBitBoard`
- `test_getPieceColorBitBoard`
- `test_getPieceBoardFromIndex`
- `test_getCurrentPlayer`
- `test_getPieceFromCoords`
- `test_getBit`
- `test_getMaterialCount`
- `test_addPiece`
- `test_removePiece`
- `test_movePiece`
- `test_forceUndo`
- `test_getCardinals`
- `test_getDiagonals`
- `test_getKnightPattern`
- `test_getKingPattern`
- `test_getPawnPattern`
- `test_getLegalMoves`
- `test_naiveCheckCheck
- `test_fromFEN`
- `test_toFEN`

