# Simple unit tests for CFBoard

--To compile: `g++ -std=c++11 -O2 main.cpp test_from_fen.cpp test_naive_check_check.cpp test_to_fen.cpp ../../lib/board_implementation/CFBoard.cpp ../../lib/board_implementation/naiveCheckCheck.cpp -o tests`--

Now replaced with Catch2 testing framework.

This tests:

- `testFromFEN`
- `testToFEN`
- `testNaiveCheckCheck`
