//
//  main.cpp
//  Generator
//
//  Created by Ariel Flahaut on 23/11/2022.
//


#include "generator.hpp"
#include <iostream>

int main(int argc, const char * argv[]) {
    file_generator_general(500);
    file_generator_6_white_6_black(500);
    file_generator_100_closed(500);
    file_generator_general_spaced(500);
    file_generator_0_to_4(500);
    return 0;
}
