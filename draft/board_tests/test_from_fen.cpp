#include "test_from_fen.h"

namespace CFBoardTester {
void testFromFEN() {
    std::vector<std::pair<std::string, CFBoard>> tests = {std::make_pair(
        std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
        CFBoard())};
    std::cout << "testFromFEN ";
    std::cout << "[";
    for (auto test : tests) {
        std::string fen = test.first;
        CFBoard board = test.second;
        if (board == CFBoard(fen)) {
            std::cout << "P";
        } else {
            std::cout << "-";
        }
    }
    std::cout << "]" << std::endl;
}
} // namespace CFBoardTester

//NOT NECESSARY - Merged into test_Board_Implementation 
