#include <iostream>

//#include "config.hpp"
#include "SwitchMain.h"

int main() {
    srand(time(NULL));

    double ClosenessCoef;
    double BestClosedfishScore, BestStockfishScore;

    BestStockfishScore = ((double) rand() / RAND_MAX) + 1;
    BestClosedfishScore = ((double) rand() / RAND_MAX) + 1;
    ClosenessCoef = ((double) rand() / RAND_MAX) + 1;

    if(ClosenessCoef * BestStockfishScore > (1 - ClosenessCoef) * BestClosedfishScore) {
        std::cout << "We choose Closedfish" << std::endl;
    } else {
        std::cout << "We choose Stockfish" << std::endl;
    }

    OutputToUI();

    return 0;
}