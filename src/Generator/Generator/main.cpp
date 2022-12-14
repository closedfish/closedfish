//
//  main.cpp
//  Generator
//
//  Created by Ariel Flahaut on 23/11/2022.
//


#include "generator.hpp"
#include <iostream>

int main(int argc, const char * argv[]) {
    file_generator_general(50);
    file_generator_6_white_6_black(50);
    file_generator_100_closed(50);
    return 0;
}
