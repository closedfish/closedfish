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

/**
 @brief this function has the goal of taking an amount of pawns (even or odd) and returning a random board with those pawns
 
@param pawns which is the number of pawns you want on your board
 
 
@return Chessboard with those pawns in the board
 */
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
             position_array.board[random_pos].piece_color=1;
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


/**
@brief this function has the goal of adding  randomly chosen numbers of king, queen, bishop, knight, and rook to an initial chessboard of pawns.

 */


void Chessboard::completion(){
    

    int rand_knight = rand()%3;
    int knightcounter=0;
    while(knightcounter<=rand_knight){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
            board[random_pos].piece=4;
            board[random_pos].piece_color=0;
            knightcounter+=1;
        }
    }
    

    int rand_bishop = rand()%3;
    int bishopcounter=0;
    while(bishopcounter<=rand_bishop){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=3;
           board[random_pos].piece_color=0;
            bishopcounter+=1;
        }
    }
    
    int rand_rook = rand()%3;
    int rookcounter=0;
    while(rookcounter<=rand_rook){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=5;
           board[random_pos].piece_color=0;
            rookcounter+=1;
        }
    }
                    
                    
    int kingcounter=0;
    while(kingcounter<=1){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=1;
           board[random_pos].piece_color=0;
            kingcounter+=1;
        }
    }
    
                                
    int rand_queen = rand()%2;
    int queencounter=0;
    while(queencounter<=rand_queen){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=2;
           board[random_pos].piece_color=0;
            queencounter+=1;
        }
    }
        
        rand_knight = rand()%3;
        while(knightcounter<=rand_knight){
            int random_pos=rand()%64;
            if(board[random_pos].is_empty()){
               board[random_pos].piece=4;
               board[random_pos].piece_color=1;
                knightcounter+=1;
            }
        }
        
        rand_bishop = rand()%3;
        while(bishopcounter<=rand_bishop){
            int random_pos=rand()%64;
            if(board[random_pos].is_empty()){
               board[random_pos].piece=3;
               board[random_pos].piece_color=1;
                bishopcounter+=1;
            }
        }
    
        rand_rook = rand()%3;
        while(rookcounter<=rand_rook){
            int random_pos=rand()%64;
            if(board[random_pos].is_empty()){
               board[random_pos].piece=5;
               board[random_pos].piece_color=1;
                rookcounter+=1;
            }
        }
                        
                        
    int kingcounter_black=0;
    while(kingcounter<=1){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=1;
           board[random_pos].piece_color=1;
            kingcounter_black+=1;
        }
    }
        
                            
        rand_queen = rand()%2;
        while(queencounter<=rand_queen){
            int random_pos=rand()%64;
            if(board[random_pos].is_empty()){
               board[random_pos].piece=2;
               board[random_pos].piece_color=1;
                queencounter+=1;
            }
        }
                        
                    
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
