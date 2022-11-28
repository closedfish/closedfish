/**
 * try_uci.cpp -- Try calling stockfish using UCI protocol
 * NOTE: This file is currently unused, since manual piping is more difficult
 * than calling the libraries directly.
 */

#include <cstdio>
#include <iostream>
#include <vector>

class Board {
    // Placeholder class to store chessboard
};

class Move {
    // Placeholder class to store a single chess move
};

class UCICommunicator {
  public:
    // ref:
    // https://stackoverflow.com/questions/6171552/popen-simultaneous-read-and-write
    UCICommunicator(const char *stockfish_path) {
        stockfish = popen(stockfish_path, "w");
        if (stockfish == NULL) {
            std::cerr << "Stockfish cannot be opened" << std::endl;
            exit(1);
        }
    }
    ~UCICommunicator() { pclose(stockfish); }
    std::vector<Move> retrieve_moves(Board b) {
        const int ST_MAX = 512;
        char st[ST_MAX];
        while (fgets(st, ST_MAX, stockfish) != NULL) {
            std::cout << st << std::endl;
        }
        fputs("help", stockfish);
        fflush(stockfish);
        while (fgets(st, ST_MAX, stockfish) != NULL) {
            std::cout << st << std::endl;
        }
    }

  private:
    FILE *stockfish;
};

int main() { UCICommunicator comm("./Stockfish/src/stockfish"); }
