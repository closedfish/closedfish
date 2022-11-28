//
//  generator.cpp
//  generator
//
//  Created by Ariel Flahaut on 27/11/2022.
//

#include "generator.hpp"
#include <iostream>     // std::cout, std::fixed
#include <iomanip>      // std::setprecision
#include <math.h>       // sin, cos
#include <limits>       // numeric_limits
#include <cmath>
#include <algorithm>
#include <array>        //for std::array
#include <fstream>      //For files
#include <stdlib.h>     // include rand
using namespace std; //removes the need to type std::


Chessboard single_generator(int pawns){ //To be fixed
    Chessboard position_array;
    int white_counter=0;
    int fullcolumns_white[]={0,0,0,0,0,0,0,0};
    int fullcolumns_black[]={0,0,0,0,0,0,0,0};
    while(white_counter<=pawns/2){
         int random_pos=rand()%64;
         if (position_array.board[random_pos].is_empty() and fullcolumns_white[random_pos%8]<2){
             position_array.board[random_pos].piece=6;
             position_array.board[random_pos].piece_color=0;
             white_counter+=1;
             fullcolumns_white[random_pos%8]+=1;           //make sure there are no more than two white pawns per column
         }
     }
    int black_counter=0;
    while(black_counter<=pawns/2){
         int random_pos=rand()%64;
         if (position_array.board[random_pos].is_empty() and fullcolumns_black[random_pos%8]<2){
             position_array.board[random_pos].piece=6;
             position_array.board[random_pos].piece_color=0;
             black_counter+=1;
             fullcolumns_black[random_pos%8]+=1;           //make sure there are no more than two black pawns per column
         }
     }
    return position_array;
}

void file_generator(int pawns){ //To be finished
    std::fstream file;
    std::ofstream outfile ("generated_position.txt");
}

void database_edit(){ //To be finished
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

double Chessboard::closeness(Chessboard){ //To be done
    
    return 1.0;
}
double Chessboard::openness(Chessboard){ //To be done
    return 1.0;
}

ArrayElement::ArrayElement() :ArrayElement(0,0){
}
ArrayElement::ArrayElement(int piece, int piece_color){
    piece=0;
    piece_color=0;
}
int ArrayElement::get_piece(){
    return piece;
}

int ArrayElement::get_piece_color(){
    return piece_color;
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
Chessboard Chessboard::replace_element(int position, ArrayElement element, Chessboard input){
    board[position]=element;
    return input;
}
int Chessboard::get_total_pawns(Chessboard input){
    int count=0;
    for (int i =0; i<64; i++){
        if(input.board[i].get_piece()==6){
            count+=1;
        }
    }
    return count;
}

int Chessboard::get_white_pawns(Chessboard input){
    int count=0;
    for (int i=0; i<64; i++){
        if (input.board[i].get_piece()==6 and input.board[i].get_piece_color()==0){
            count+=1;
        }
    }
    return count;
}
int Chessboard::get_black_pawns(Chessboard input){
    int count=0;
    for (int i=0; i<64; i++){
        if (input.board[i].get_piece()==6 and input.board[i].get_piece_color()==1){
            count+=1;
        }
    }
    return count;
}
