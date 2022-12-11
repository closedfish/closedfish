//
//  main.cpp
//  Generator
//
//  Created by Ariel Flahaut on 23/11/2022.
//


#include "generator.hpp"
#include <iostream>
int main(int argc, const char * argv[]) { // testing
    Chessboard chessboard;                // generate instance of chessboard class
    chessboard.single_generator(16);      // generate 16 random pawn positions
    chessboard.visualize();               // visualize the board and said positions
    chessboard.closeness(chessboard);
    return 0;
}
