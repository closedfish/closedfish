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


 Chessboard single_generator(int pawns){
     Chessboard position_array;
     for (int i=0; i<=pawns; i++){
         int random_pos=rand()%64;
         if (position_array.board[random_pos].is_empty()){
             position_array.board[random_pos].piece=0;
             position_array.board[random_pos].piece_color=0;
             
             position_array.board[random_pos].square_color=0;

         }
 
     }
        
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

Chessboard completion(Chessboard){
//    int* insertX(int n, int array[], int pos);
//    int n=0;
//        n++;
//        for (int i=n; i>=pos; i--) {
//            if(array[i].is_empty()){
//
//                int rand1 = rand()%3;
//                arr[pos - 1] = rand1;
//                int rand2 = rand()%3;
//                int rand3 = rand()%3;
//                int rand4 = rand()%2;
//                int rand5 = rand()%2;
//                array[i] = array[i - 1];
//
    Chessboard position_array;
    return position_array;
}

double closeness(std::array<int,64>){
    
    return 1.0;
}
double openness(std::array<int,64>){
    return 1.0;
}

ArrayElement::ArrayElement() :ArrayElement(0,0,0){
}

int ArrayElement::get_piece(){
    return piece;
}

int ArrayElement::get_piece_color(){
    return piece_color;
}
int ArrayElement::get_square_color(){
    return square_color;
}



bool ArrayElement::is_empty(){
    if (piece == 0){
        return true;
    }
    else {
        return false;
        }
}



Chessboard::Chessboard(){
    for (int i =0;i<64; i++){
        board[i]=element;
        
    }
}
Chessboard Chessboard::replace_element(int position, ArrayElement element){
    board[position]=element;
}


