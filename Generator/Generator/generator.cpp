//
//  generator.cpp
//  Generator
//
//  Created by Ariel Flahaut on 23/11/2022.
//
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision
#include <math.h>       // sin, cos
#include <limits>       // numeric_limits
#include <cmath>
#include <algorithm>
#include <array>        //for std::array
#include "generator.hpp"
#include <fstream>      //For files

std::array<int, 64> single_generator(int pawns){
    std::array<int,64> position_array;
    return position_array;
}

void file_generator(int pawns){
    std::fstream file;
    std::ofstream outfile ("generated_position.txt");
}

void database_edit(){
    std::fstream file;
    std::ofstream outfile ("new_database.txt");
}

std::array<int, 64> completion(std::array<int, 64> input){
    
    std::array<int, 64> position_array;
    return position_array;
}

double closeness(std::array<int,64>){
    return 1.0;
}
double openness(std::array<int,64>){
    return 1.0;
}

