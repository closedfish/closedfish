#include "test_naive_check_check.h"

namespace CFBoardTester {
void testNaiveCheckCheck() {
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "testNaiveCheckCheck ";
    std::cout << "[";
    for (auto test : tests) {
        bool isCheck = std::get<0>(test);
        if (isCheck == std::get<2>(test).naiveCheckCheck(std::get<1>(test))) {
            std::cout << "P";
        } else {
            std::cout << "-";
        }
    }
    std::cout << "]" << std::endl;
}
} // namespace CFBoardTester