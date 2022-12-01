# Simple unit tests for CFBoard

To compile: `g++ -std=c++11 -O2 main.cpp test_from_fen.cpp test_naive_check_check.cpp test_to_fen.cpp ../../lib/board_implementation/CFBoard.cpp ../../lib/board_implementation/naiveCheckCheck.cpp -o tests`

This tests:

-   `testFromFEN`
-   `testToFEN`
-   `testNaiveCheckCheck`

To read the result:

-   `[<codes>]` stands for one subtask
-   `<codes>` is a string of `<code>` characters
-   if `<code>` is `P`, this passes the test
-   if `<code>` is `-`, this is a failure
