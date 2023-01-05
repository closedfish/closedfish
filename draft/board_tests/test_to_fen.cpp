#include "test_from_fen.h"

namespace CFBoardTester {
void testToFEN() {
    std::vector<std::pair<std::string, CFBoard>> tests = {std::make_pair(
        std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
        CFBoard())};
    std::cout << "testToFEN ";
    std::cout << "[";
    for (auto test : tests) {
        std::string fen = test.first;
        std::string ret = test.second.toFEN();
        if (ret == fen) {
            std::cout << "P";
        } else {
            std::cout << "-";
        }
    }
    std::cout << "]" << std::endl;
}
} // namespace CFBoardTester