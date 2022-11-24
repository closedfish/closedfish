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
#include <stdlib.h>     // include rand

using namespace std; //removes the need to type std::


// Chessboard single_generator(int pawns){
//     Chessboard position_array;
//    for (int i=0; i<=pawns; i++){
//        int random_pos=rand()%64;
//        if (position_array[random_pos].is_empty()){
//
//        }
//
//    }
        
//    return position_array;
//}

void file_generator(int pawns){
    std::fstream file;
    std::ofstream outfile ("generated_position.txt");
}

void database_edit(){
    std::fstream file;
    std::ofstream outfile ("new_database.txt");
    
}






double closeness(std::array<int,64>){
    
    return 1.0;
}
double openness(std::array<int,64>){
    return 1.0;
}




