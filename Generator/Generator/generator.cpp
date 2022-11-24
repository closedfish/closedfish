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
    int* insertX(int n, int array[], int pos)
        int i;
        n++;
        for (i=n; i>=pos; i--) {
            if(array[i].is_empty()){
                
                int rand1 = rand()%3;
                arr[pos - 1] = rand1;
                int rand2 = rand()%3;
                int rand3 = rand()%3;
                int rand4 = rand()%2;
                int rand5 = rand()%2;
                array[i] = array[i - 1];
    
    std::array<int, 64> position_array;
    return position_array;
}

double closeness(std::array<int,64>){
    
    return 1.0;
}
double openness(std::array<int,64>){
    return 1.0;
}

