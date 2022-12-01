//
//  generator.cpp
//  generator
//
//  Created by Ariel Flahaut on 27/11/2022.
//

#include "generator.hpp"
#include <fstream>      //For files
#include <stdlib.h>     // include rand
#include <iostream>
using namespace std; //removes the need to type std::

/**
@brief this function has the goal of taking an amount of pawns (even or odd) and returning a random board with those pawns
 
@param pawns which is the number of pawns you want on your board for now it should be even

 */
void Chessboard::single_generator(int pawns){ //To be fixed
    int white_counter=0;
    int fullcolumns_white[]={0,0,0,0,0,0,0,0};
    if (pawns%2!=0 or pawns>16 or pawns<0){
        std::cout<<"Please input a correct amount of pawns for completely closed positions i.e even number and less than 16 and more than 0. The board will not be generated."<<std::endl;
    }
    else{
        while(white_counter<=pawns/2){
            int random_pos=rand()%64;
            if ((board[random_pos].is_empty()) and (fullcolumns_white[random_pos%8]<2) and (random_pos>8) and random_pos<48){
                board[random_pos].piece=6;
                board[random_pos].piece_color=0;
                white_counter+=1;
                fullcolumns_white[random_pos%8]+=1;           //make sure there are no more than two white pawns per column
            }
        }
        for (int i=0; i<64;i++){
            if (board[i].piece==6 and board[i].piece_color==0){
                board[i+8].piece=6;
                board[i+8].piece_color=1;

        }
    }
    }

}

void file_generator(int pawns){ //To be finished
    std::fstream file;
    std::ofstream outfile ("generated_position.txt");
}

/**
@brief this function has the goal of taking a .cbv chess database and creating a copy of said database with only pawns
 
 */
void database_edit(){ //To be finished
    std::fstream file;
    std::ofstream outfile ("new_database.txt");
    
}


/**
@brief this function has the goal of adding  randomly chosen numbers of king, queen, bishop, knight, and rook to an initial chessboard of pawns.


 */


void Chessboard::completion(){
    

    int rand_knight = rand()%3; // choose a random number between 0 and 2 of white knights.
    int knightcounter_white=0;
    while(knightcounter_white<=rand_knight){ // find empty places to attend.
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
            board[random_pos].piece=4;
            board[random_pos].piece_color=0;
            knightcounter_white+=1;
        }
    }
    

    int rand_bishop = rand()%3;
    int bishopcounter_white=0;
    while(bishopcounter_white<=rand_bishop){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=3;
           board[random_pos].piece_color=0;
            bishopcounter_white+=1;
        }
    }
    
    int rand_rook = rand()%3;
    int rookcounter_white=0;
    while(rookcounter_white<=rand_rook){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=5;
           board[random_pos].piece_color=0;
            rookcounter_white+=1;
        }
    }
                    
                    
    int kingcounter_white=0;
    while(kingcounter_white<=1){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=1;
           board[random_pos].piece_color=0;
            kingcounter_white+=1;
        }
    }
    
                                
    int rand_queen = rand()%2;
    int queencounter_white=0;
    while(queencounter_white<=rand_queen){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=2;
           board[random_pos].piece_color=0;
            queencounter_white+=1;
        }
    }
        
    rand_knight = rand()%3; // choose a random number between 0 and 2 of black knights.
    int knightcounter_black=0;
        while(knightcounter_black<=rand_knight){ // find empty places to attend.
            int random_pos=rand()%64;
            if(board[random_pos].is_empty()){
               board[random_pos].piece=4;
               board[random_pos].piece_color=1;
                knightcounter_black+=1;
            }
        }
        
    rand_bishop = rand()%3;
    int bishopcounter_black=0;
        while(bishopcounter_black<=rand_bishop){
            int random_pos=rand()%64;
            if(board[random_pos].is_empty()){
               board[random_pos].piece=3;
               board[random_pos].piece_color=1;
                bishopcounter_black+=1;
            }
        }
    
    rand_rook = rand()%3;
    int rookcounter_black=0;
        while(rookcounter_black<=rand_rook){
            int random_pos=rand()%64;
            if(board[random_pos].is_empty()){
               board[random_pos].piece=5;
               board[random_pos].piece_color=1;
                rookcounter_black+=1;
            }
        }
                        
                        
    int kingcounter_black=0;
    while(kingcounter_black<=1){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){
           board[random_pos].piece=1;
           board[random_pos].piece_color=1;
            kingcounter_black+=1;
        }
    }
        
                            
        rand_queen = rand()%2;
    int queencounter_black =0;
        while(queencounter_black<=rand_queen){
            int random_pos=rand()%64;
            if(board[random_pos].is_empty()){
               board[random_pos].piece=2;
               board[random_pos].piece_color=1;
                queencounter_black+=1;
            }
        }
                        
                    
                    }
                   
                
            

/**
@brief this function has the goal of taking a chess position in the form of our chessboard array and returning the coefficient of closeness
@param input array
@return coefficient of closeness
 */
double Chessboard::closeness(Chessboard input){ //Etienne, NOT FINISHED, flaw in logic
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
@brief this function has the goal of taking a chess position in the form of our chessboard array and returning the coefficient of openness
@param input array
@return coefficient of openness
 */

double Chessboard::openness(Chessboard input){ //Etienne, NOT FINISHED, flaw in logic 
    int tot_pawn = get_total_pawns(input);
    int tot_white = get_white_pawns(input);
    int tot_black = get_black_pawns(input);
    
    int closed_pawns = 0;
    int open_pawns = 0;
    
    for (int i = 0; i < 64; i++){
        if (input.board[i].get_piece() == 6){ // if a pawn
            
            if (input.board[i].get_piece_color() == 1){ // if black (check status going down array)
                if (input.board[i+8].get_piece() != 6){ // if no blocking pawn in front (8 positions down the array)
                    open_pawns += 1;
                }
                
                else {
                    
                    if (i%8 == 1 && input.board[i+9].get_piece() == 6){ // if pawn along "a" column with a pawn to lower right
                        open_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i+7].get_piece() == 6){ // if pawn along "h" column with a pawn to lower left
                        open_pawns += 1;
                    }
                    
                    else if (input.board[i+9].get_piece() == 6 ||
                             input.board[i+7].get_piece() == 6){ // if pawn not along edges, with a pawn to either its lower left or right (or both)
                        open_pawns += 1;
                    }
                }
            }
            
            if (input.board[i].get_piece_color() == 1){ // if black (check status going down array)
                if (input.board[i-8].get_piece() != 6){ // if no blocking pawn in front (8 positions down the array)
                    open_pawns += 1;
                }
                
                else {
                    
                    if (i%8 == 1 && input.board[i-7].get_piece() == 6){ // if pawn along "a" column with a pawn to upper right
                        open_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i-9].get_piece() == 6){ // if pawn along "h" column with a pawn to upper left
                        open_pawns += 1;
                    }
                    
                    else if (input.board[i-9].get_piece() == 6 ||
                             input.board[i-7].get_piece() == 6){ // if pawn not along edges, with a pawn to either its upper left or right (or both)
                        open_pawns += 1;
                    }
                }
            }
        }
    }
    return open_pawns/tot_pawn;
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
        board[i].piece=0;
        board[i].piece_color=0;
        
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
        if (input.board[i].piece==6 and input.board[i].piece_color==0){
            count+=1;
        }
    }
    return count;
}
int Chessboard::get_black_pawns(Chessboard input){
    int count=0;
    for (int i=0; i<64; i++){
        if (input.board[i].piece==6 and input.board[i].piece_color==1){
            count+=1;
        }
    }
    return count;
}

void Chessboard::visualize(){
    int count=0;
    for (int i =0; i<64; i++){
        cout<<"["<<board[i].piece<<","<<board[i].piece_color<<"] ";
        count++;
        if (count%8==0){
            cout<<endl;
        
        }
        
    }
}
