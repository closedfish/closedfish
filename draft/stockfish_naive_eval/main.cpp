#include "stockfish_naive_eval.h"
#include <iostream>
#include <tuple>
#include <vector>

/**
 * @brief Lightweight unit test
 *
 * @param testcases
 */
void test(std::vector<std::tuple<std::string, int>> testcases) {
    for (auto testcase : testcases) {
        std::string fen;
        int expected;
        tie(fen, expected) = testcase;
        CFBoard board(fen);
        int outcome = stockfishNaiveEval(board);
        if (outcome == expected) {
            std::cout << "[SUCCESS] on input " << fen << " with output "
                      << expected << std::endl;
        } else {
            std::cout << "[WRONG] on input " << fen << " expected " << expected
                      << " retrieved " << outcome << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    Stockfish::CommandLine::init(argc, argv);
    Stockfish::UCI::init(Stockfish::Options);
    Stockfish::Tune::init();
    Stockfish::PSQT::init();
    Stockfish::Bitboards::init();
    Stockfish::Position::init();
    Stockfish::Bitbases::init();
    Stockfish::Endgames::init();
    Stockfish::Threads.set(size_t(Stockfish::Options["Threads"]));
    Stockfish::Search::clear(); // After threads are up
    Stockfish::Eval::useNNUE = false;
    // Stockfish::Eval::NNUE::init();
    Stockfish::Position::init(); // if not yet init (should rather be inside the
                                 // main function)
    test({{"rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP3K1/1P6/RNBQ1B1R w - - 0 1",
           316},
          {"rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP1K2/PpP1N3/1P6/RNBQ1BR1 w - - 0 1",
           12}});
}