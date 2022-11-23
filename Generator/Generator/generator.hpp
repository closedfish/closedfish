//
//  generator.hpp
//  Generator
//
//  Created by Ariel Flahaut on 23/11/2022.
//

#ifndef generator_hpp
#define generator_hpp

#include <stdio.h>

#endif /* generator_hpp */
std::array<int, 64> single_generator(int pawns); //Input number of pawns and return a random closed position with only pawns

void file_generator(int pawns); //Input number of pawns and return a file with every possible closed position with only pawns


std::array<int, 64> completion(std::array<int, 64>);// Shirelle, input only pawns and fill the board with other pieces randomly


double closeness(std::array<int,64>); //input a position return a percentage

double openness(std::array<int,64>); //input a position return a percentage

