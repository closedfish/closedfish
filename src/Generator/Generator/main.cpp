//
//  main.cpp
//  Generator
//
//  Created by Ariel Flahaut on 23/11/2022.
//


#include "generator.hpp"
#include <iostream>
int main(int argc, const char * argv[]) {
    Chessboard chessboard;
    chessboard.visualize();
    chessboard.single_generator(16);
    chessboard.visualize();
    int i =chessboard.find_pawn_in_column(chessboard, 6, 1);
    cout<<i;

    return 0;
}
