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
std::array<int, 64> generator(int pawns); //Input number of pawns and return a random closed position

double closeness(std::array<int,64>); //input a position return a percentage

double openness(std::array<int,64>); //input a position return a percentage

