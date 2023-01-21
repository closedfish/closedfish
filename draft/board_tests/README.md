# Simple unit tests for CFBoard

To compile: `g++ -std=c++11 -O2 main.cpp test_from_fen.cpp test_naive_check_check.cpp test_to_fen.cpp ../../lib/board_implementation/CFBoard.cpp ../../lib/board_implementation/naiveCheckCheck.cpp -o tests`

This tests:
-   `test_pieceIdToChar`
-   `test_pieceCharToId`
-   `test_getColorBitBoard`
-   `test_getPieceColorBitBoard`
-   `test_getPieceBoardFromIndex`
-   `test_getCurrentPlayer`
-   `test_getPieceFromCoords`
-   `test_getBit`
-   `test_getMaterialCount`
-   `test_addPiece`
-   `test_removePiece`
-   `test_movePiece`
-   `test_forceUndo`
-   `test_getCardinals`
-   `test_getDiagonals`
-   `test_getKnightPattern`
-   `test_getKingPattern`
-   `test_getPawnPattern`
-   `test_getLegalMoves`
-   `test_NaiveCheckCheck`
-   `test_FromFEN`
-   `test_ToFEN`

To read the result:

-   `[<codes>]` stands for one subtask
-   `<codes>` is a string of `<code>` characters
-   if `<code>` is `Success!`, this passes the test
-   if `<code>` is `Failure :(`, this is a failure
