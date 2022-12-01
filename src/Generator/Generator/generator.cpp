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

/**
 @brief this function has the goal of taking a .cbv chess database and creating a copy of said database with only pawns
 
@param the chess database
 
 
@return a .cbv file containing the same moves, excluding pieces other than pawns
 */
void database_edit(){ //To be finished
    std::fstream file;
    std::ofstream outfile ("new_database.txt");
    
}

/**
@brief generates a random chessboard with randomly chosen numbers of king, queen, bishop, knight, and rook.

@param  input

 */


Chessboard completion(Chessboard input){

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

/**
@brief this function has the goal of taking a chess position in the form of our chessboard array and returning the coefficient of closeness
@param input array
@return Chessboard with those pawns in the board
 */
double Chessboard::closeness(Chessboard input){ //To be done, Etienne
    int tot_pawn = get_total_pawns(input);
    int tot_white = get_white_pawns(input);
    int tot_black = get_black_pawns(input);
    
    int closed_pawns = 0;
    int open_pawns = 0;
    
    for (int i = 0; i < 64; i++){
        if (input.board[i].get_piece() == 6){ // if a pawn
            
            if (input.board[i].get_piece_color() == 1){ // if black (check status going down array)
                if (input.board[i+8].get_piece() == 6){ // if blocking pawn in front (8 positions down the array)
                    
                    if (i%8 == 1 && input.board[i+9].get_piece() != 6){ // if pawn along "a" column without a pawn to lower right
                        closed_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i+7].get_piece() != 6){ // if pawn along "h" column without a pawn to lower left
                        closed_pawns += 1;
                    }
                    
                    else if (input.board[i+9].get_piece() != 6 && // if pawn not along edges, without a pawn to lower left or right
                             input.board[i+7].get_piece() != 6){
                        closed_pawns += 1;
                    }
                }
            }
            
            if (input.board[i].get_piece_color() == 0){
                if (input.board[i-8].get_piece() == 6){ // if blocking pawn in front (8 positions up the array)
                    
                    if (i%8 == 1 && input.board[i-7].get_piece() != 6){ // if pawn along "a" column without a pawn to upper right
                        closed_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i-9].get_piece() != 6){ // if pawn along "h" column without a pawn to upper left
                        closed_pawns += 1;
                    }
                    
                    else if (input.board[i-9].get_piece() != 6 && // if pawn not along edges, without a pawn to lower left or right
                             input.board[i-7].get_piece() != 6){
                        closed_pawns += 1;
                    }
                }
            }
        }
    }
    return closed_pawns/tot_pawn;
}

/**
@brief this function has the goal of taking a chess position in the form of our chessboard array and returning the coefficient of closeness
@param input array
@return Chessboard with those pawns in the board
 */

double Chessboard::openness(Chessboard input){ //To be done, Etienne
    int tot_pawn = get_total_pawns(input);
    int tot_white = get_white_pawns(input);
    int tot_black = get_black_pawns(input);
    
    int closed_pawns = 0;
    int open_pawns = 0;
    
    for (int i = 0; i < 64; i++){
        if (input.board[i].get_piece() == 6){ // if a pawn
            
            if (input.board[i].get_piece_color() == 1){ // if black (check status going down array)
                if (input.board[i+8].get_piece() == 6){ // if blocking pawn in front (8 positions down the array)
                    
                    if (i%8 == 1 && input.board[i+9].get_piece() != 6){ // if pawn along "a" column without a pawn to lower right
                        closed_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i+7].get_piece() != 6){ // if pawn along "h" column without a pawn to lower left
                        closed_pawns += 1;
                    }
                    
                    else if (input.board[i+9].get_piece() != 6 && // if pawn not along edges, without a pawn to lower left or right
                             input.board[i+7].get_piece() != 6){
                        closed_pawns += 1;
                    }
                }
            }
            
            if (input.board[i].get_piece_color() == 0){
                if (input.board[i-8].get_piece() == 6){ // if blocking pawn in front (8 positions up the array)
                    
                    if (i%8 == 1 && input.board[i-7].get_piece() != 6){ // if pawn along "a" column without a pawn to upper right
                        closed_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i-9].get_piece() != 6){ // if pawn along "h" column without a pawn to upper left
                        closed_pawns += 1;
                    }
                    
                    else if (input.board[i-9].get_piece() != 6 && // if pawn not along edges, without a pawn to lower left or right
                             input.board[i-7].get_piece() != 6){
                        closed_pawns += 1;
                    }
                }
            }
        }
    }
    return closed_pawns/tot_pawn;
}
    return r;
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
