#include "test_from_fen.h"
#include "test_naive_check_check.h"
#include "test_to_fen.h"

int main() {
    CFBoardTester::testFromFEN();
    CFBoardTester::testToFEN();
    CFBoardTester::testNaiveCheckCheck();
    return 0;
}