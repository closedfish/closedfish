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
#include <vector>
using namespace std; //removes the need to type std::

/**
@brief This function takes no input, generates a chessboard closed position and converts it into an output for cassidy.

@return This function returns a two dimensional vector where each vector element of the initial element is of the form {xcoord, ycoord, piece_color} for each column, if a specific column has no pawn then both the black and white pawn in that column will have values {9,9,9}.

 */

std::vector<std::vector<int>> listpawns(){
    std::vector<std::vector<int>> listpawn
    {{-1,-1,-1,-1,-1,-1,-1,-1}, {8,8,8,8,8,8,8,8}};
    Chessboard chessboard;
    int pawns_num=rand()%6;
    pawns_num+=10;
    if (pawns_num%2!=0){pawns_num+=1;}
    chessboard.single_generator(pawns_num);
    int color=-1;
    for (int i = 0; i < listpawn.size(); i++){
        color+=1;
        for(int j = 0; j < listpawn[i].size(); j++){
            int a=chessboard.find_pawn_in_column(chessboard, j, color);
            listpawn[i][j]=a;
        }
    }
    return listpawn;
}
std::vector<std::vector<int>> listpawnsclosed(){
    std::vector<std::vector<int>> listpawn
    {{-1,-1,-1,-1,-1,-1,-1,-1}, {8,8,8,8,8,8,8,8}};
    Chessboard chessboard;
    int pawns_num=16;
    chessboard.single_generator(pawns_num);
    int color=-1;
    for (int i = 0; i < listpawn.size(); i++){
        color+=1;
        for(int j = 0; j < listpawn[i].size(); j++){
            int a=chessboard.find_pawn_in_column(chessboard, j, color);
            listpawn[i][j]=a;
        }
    }
    return listpawn;
}
std::vector<std::vector<int>> listpawns_6_white_6_black(){
    std::vector<std::vector<int>> listpawn
    {{-1,-1,-1,-1,-1,-1,-1,-1}, {8,8,8,8,8,8,8,8}};
    Chessboard chessboard;
    int pawns_num=12;
    chessboard.single_generator(pawns_num);
    int color=-1;
    for (int i = 0; i < listpawn.size(); i++){
        color+=1;
        for(int j = 0; j < listpawn[i].size(); j++){
            int a=chessboard.find_pawn_in_column(chessboard, j, color);
            listpawn[i][j]=a;
        }
    }
    return listpawn;
}

/**
@brief this function has the goal of taking an amount of pawns (even or odd) and returning a random board with those pawns
 
@param pawns which is the number of pawns you want on your board for now it should be even

 */
void Chessboard::single_generator(int pawns){ //To be fixed
    int white_counter=0;
    int fullcolumns_white[]={0,0,0,0,0,0,0,0};
    if (pawns%2!=0 or pawns>16 or pawns<0){ // check for correct number of pawns (needs to be even, and within 1-16)
        std::cout<<"Please input a correct amount of pawns for completely closed positions i.e even number and less than 16 and more than 0. The board will not be generated."<<std::endl;
    }
    else{
        while(white_counter<pawns/2){
            int random_pos=rand()%48;
            if ((board[random_pos].is_empty()) and (fullcolumns_white[(random_pos%8)]!=1) and (random_pos>8)){
                board[random_pos].piece=6;
                board[random_pos].piece_color=0;
                white_counter+=1;
                fullcolumns_white[(random_pos%8)]+=1;           //make sure there are no more than two white pawns per column
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

/**
@brief Creates a file of mostly closed positions in the vector in vector output format for the switch algo.
@param position_amount The parameter here is the amount of positions you want to have generated in the file.
 */
void file_generator_general(int position_amount){
    ofstream file;
    file.open ("closed_positions.txt");
    int counter=-1;
    for (int i=0; i<position_amount;i++){
        string output="";
        std::vector<std::vector<int>> theboard = listpawnsclosed();
        for (int i = 0; i < theboard.size(); i++){
            counter+=1;
            int count=0;
            output+="\n";
            output+="X[";
            output+=to_string(counter);
            output+="] = ";
            output+="new int[8]{";
            for(int j = 0; j < theboard[i].size(); j++){
                output+=to_string(theboard[i][j]);
                count+=1;
                if (count<8){
                    output+=", ";
                }
            }
            output+="};";
        }
        cout<<output;
        file<<output<<endl;
    }
    file.close();
}

/**
@brief Creates a file of completely closed positions in the vector in vector output format for the switch algo.
@param position_amount The parameter here is the amount of positions you want to have generated in the file.
 */
void file_generator_100_closed(int position_amount){
    ofstream file;
    file.open ("closed_positions.txt");
    int counter=-1;
    for (int i=0; i<position_amount;i++){
        string output="";
        std::vector<std::vector<int>> theboard = listpawnsclosed();
        for (int i = 0; i < theboard.size(); i++){
            counter+=1;
            int count=0;
            output+="\n";
            output+="X[";
            output+=to_string(counter);
            output+="] = ";
            output+="new int[8]{";
            for(int j = 0; j < theboard[i].size(); j++){
                output+=to_string(theboard[i][j]);
                count+=1;
                if (count<8){
                    output+=", ";
                }
            }
            output+="};";
        }
        cout<<output;
        file<<output<<endl;
    }
    file.close();
}

/**
@brief Creates a file of completely closed positions in the vector in vector output format for the switch algo.
@param position_amount The parameter here is the amount of positions you want to have generated in the file.
 */
void file_generator_6_white_6_black(int position_amount){
    ofstream file;
    file.open ("closed_positions.txt");
    int counter=-1;
    for (int i=0; i<position_amount;i++){
        string output="";
        std::vector<std::vector<int>> theboard = listpawns_6_white_6_black();
        for (int i = 0; i < theboard.size(); i++){
            counter+=1;
            int count=0;
            output+="\n";
            output+="X[";
            output+=to_string(counter);
            output+="] = ";
            output+="new int[8]{";
            for(int j = 0; j < theboard[i].size(); j++){
                output+=to_string(theboard[i][j]);
                count+=1;
                if (count<8){
                    output+=", ";
                }
            }
            output+="};";
        }
        cout<<output;
        file<<output<<endl;
    }
    file.close();
}
/**
@brief this function has the goal of taking a .cbv chess database and creating a copy of said database with only pawns
@param .cbv file  which holds millions of chess positions
 */

void database_edit(){ //Will not be implemented
    std::fstream file;
    std::ofstream outfile ("new_database.txt");
}

/**
@brief this function has the goal of adding  randomly chosen numbers of king, queen, bishop, knight, and rook to an initial chessboard of pawns.
 */

void Chessboard::completion(){
    

int rand_knight = rand()%3; // choose a random number between 0 and 2 of white knights
int knightcounter_white=0;
    while(knightcounter_white<=rand_knight){ // iterate rand_knight times
          int random_pos=rand()%64;
            if(board[random_pos].is_empty()){ // if position empty, assign white knight
            board[random_pos].piece=4;
            board[random_pos].piece_color=0;
            knightcounter_white+=1;
        }
    }
    
    
int rand_bishop = rand()%3;  // choose a random number between 0 and 2 of white bishops
int bishopcounter_white=0;
    while(bishopcounter_white<=rand_bishop){
    int random_pos=rand()%64;
        if(board[random_pos].is_empty()&& rand_bishop!=2){
            board[random_pos].piece=3;
            board[random_pos].piece_color=0;
            bishopcounter_white+=1;
        }
        random_pos=rand()%64;
        if(board[random_pos].is_empty()&& rand_bishop==2){
            for(int bishopcounter_white=0; bishopcounter_white<=1;){
                board[random_pos].piece=3;
                board[random_pos].piece_color=0;
                bishopcounter_white+=1;
            }
            random_pos=rand()%64;
            for(int bishopcounter_white=2; bishopcounter_white<=2;){
                board[random_pos].piece=3;
                board[random_pos].piece_color=0;
                board[random_pos].square_color=1;
                
            }
        }
    }
            
            
            
int rand_rook = rand()%3; // choose a random number between 0 and 2 of white rooks
int rookcounter_white=0;
    while(rookcounter_white<=rand_rook){ // iterate rand_rook times
        int random_pos=rand()%64;
            if(board[random_pos].is_empty()){ // if position empty, assign white rook
                board[random_pos].piece=5;
                board[random_pos].piece_color=0;
                rookcounter_white+=1;
                }
            }
            
            
int kingcounter_white=0;
    while(kingcounter_white<1){ // Place one white king (since without one the game is over)
        int random_pos=rand()%64;
            if(board[random_pos].is_empty()){ // if position empty, assign white king
                board[random_pos].piece=1;
                board[random_pos].piece_color=0;
                kingcounter_white+=1;
                }
            }
            
            
int rand_queen = rand()%2; // choose a random number between 0 and 2 of white queens
int queencounter_white=0;
    while(queencounter_white<=rand_queen){ // iterate rand_queen times
        int random_pos=rand()%64;
            if(board[random_pos].is_empty()){ // if position empty, assign white queen
                board[random_pos].piece=2;
                board[random_pos].piece_color=0;
                queencounter_white+=1;
                }
            }
            
int rand_knight2 = rand()%3; // choose a random number between 0 and 2 of black knights.
int knightcounter_black=0;
    while(knightcounter_black<=rand_knight2){ // find empty places to attend.
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){ // if position empty, assign black knight
            board[random_pos].piece=4;
            board[random_pos].piece_color=1;
            knightcounter_black+=1;
                }
            }
            
            
            
int rand_bishop2 = rand()%3; //choose a random number of black bishop
int bishopcounter_black=0;
    while(bishopcounter_black<=rand_bishop2){
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()&& rand_bishop2!=2){ // if there is none or 1 bishop, assign to an empty square
            board[random_pos].piece=3;
            board[random_pos].piece_color=1;
            bishopcounter_black+=1;
                }
        random_pos=rand()%64;
        if(board[random_pos].is_empty()&& rand_bishop2==2){ // if there are 2 bishops, make sure that their square                                                                        colors are different
            for(int bishopcounter_black=0; bishopcounter_black<=1;){
                board[random_pos].piece=3;
                board[random_pos].piece_color=1;
                bishopcounter_black+=1;
                    }
            random_pos=rand()%64;
            for(int bishopcounter_black=2; bishopcounter_black<=2;){
                board[random_pos].piece=3;
                board[random_pos].piece_color=1;
                board[random_pos].square_color=1;
                        
                    }
                }
            }
            
            
int rand_rook2 = rand()%3;// choose a random number between 0 and 2 of black rooks
int rookcounter_black=0;
    while(rookcounter_black<=rand_rook2){ // iterate rand_rook_black times
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){ // if position empty, assign black rook
            board[random_pos].piece=5;
            board[random_pos].piece_color=1;
            rookcounter_black+=1;
                }
            }
            
            
int kingcounter_black=0;
    while(kingcounter_black<1){ // Place one black king (since without one the game is over)
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){ // if position empty, assign black king
            board[random_pos].piece=1;
            board[random_pos].piece_color=1;
            kingcounter_black+=1;
                }
            }
            
            
            
int rand_queen2 = rand()%2;// choose a random number between 0 and 2 of black queens
int queencounter_black =0;
    while(queencounter_black<=rand_queen2){// iterate rand_queen_black times
        int random_pos=rand()%64;
        if(board[random_pos].is_empty()){ // if position empty, assign black queen
            board[random_pos].piece=2;
            board[random_pos].piece_color=1;
            queencounter_black+=1;
                }
            }
            
        
        }
        
        
    


/**
@brief this function has the goal of taking a chess position in the form of our chessboard array and returning the coefficient of closeness
@param input chessboard array
@return coefficient of closeness
 */
double Chessboard::closeness(Chessboard input){ 
    int tot_pawn = get_total_pawns(input);
    //int tot_white = get_white_pawns(input); // not being used
    //int tot_black = get_black_pawns(input);
    
    int closed_pawns = 0;
    //int open_pawns = 0;
    
    for (int i = 0; i < 64; i++){
        if (input.board[i].get_piece() == 6){ // if a pawn
            
            if (input.board[i].get_piece_color() == 1){ // if black (check status going down array)
                if (input.board[i+8].get_piece() == 6){ // if blocking pawn in front (8 positions down the array)
                    
                    if (i%8 == 1 && input.board[i+9].get_piece() != 6){ // if pawn along "a" column without a pawn to lower right
                        closed_pawns += 1;
                    }
                    
                    else if (input.board[i+9].get_piece() != 6 && // if pawn not along edges, without a pawn to lower left or right
                             input.board[i+7].get_piece() != 6){
                        closed_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i+7].get_piece() != 6){ // if pawn along "h" column without a pawn to lower left
                        closed_pawns += 1;
                    }
                    
                   
                }
            }
            
            if (input.board[i].get_piece_color() == 0){
                if (input.board[i-8].get_piece() == 6){ // if blocking pawn in front (8 positions up the array)
                    
                    if (i%8 == 1 && input.board[i-7].get_piece() != 6){ // if pawn along "a" column without a pawn to upper right
                        closed_pawns += 1;
                    }
                    
                    else if (input.board[i-9].get_piece() != 6 && // if pawn not along edges, without a pawn to lower left or right
                             input.board[i-7].get_piece() != 6){
                        closed_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i-9].get_piece() != 6){ // if pawn along "h" column without a pawn to upper left
                        closed_pawns += 1;
                    }
                    
                   
                }
            }
        }
    }
    //cout<<closed_pawns/tot_pawn; // for testing
    return closed_pawns/tot_pawn;
}

/**
@brief this function has the goal of taking a chess position in the form of our chessboard array and returning the coefficient of openness
@param input chessboard array
@return coefficient of openness
 */

double Chessboard::openness(Chessboard input){
    int tot_pawn = get_total_pawns(input);
    //int tot_white = get_white_pawns(input); // not needed
    //int tot_black = get_black_pawns(input);
    
    //int closed_pawns = 0;
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
                    
                    else if (input.board[i+9].get_piece() == 6 ||
                             input.board[i+7].get_piece() == 6){ // if pawn not along edges, with a pawn to either its lower left or right (or both)
                        open_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i+7].get_piece() == 6){ // if pawn along "h" column with a pawn to lower left
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
                    
                    else if (input.board[i-9].get_piece() == 6 ||
                             input.board[i-7].get_piece() == 6){ // if pawn not along edges, with a pawn to either its upper left or right (or both)
                        open_pawns += 1;
                    }
                    
                    else if (i%8 == 0 && input.board[i-9].get_piece() == 6){ // if pawn along "h" column with a pawn to upper left
                        open_pawns += 1;
                    }
                    
                   
                }
            }
        }
    }
    //cout<<open_pawns/tot_pawn; //for testing
    return open_pawns/tot_pawn;
}


ArrayElement::ArrayElement() :ArrayElement(0,0,0){


}

ArrayElement::ArrayElement(int piece, int piece_color, int square_color){
    piece=0;
    piece_color=0;
    square_color=0;
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
        board[i].piece=0;
        board[i].piece_color=0;
        board[i].square_color=0;
        
    }
    
    int rows=1;
    for(int i=0; i<64; i+=2){
        if(i%8==0){
            rows+=1;
        }
        if (rows%2==1){
            board[i].square_color=0;
            board[i+1].square_color=1;
        }
        if (rows%2==0){
            board[i].square_color=1;
            board[i+1].square_color=0;
            
        }
    }
    
    
}

void Chessboard::replace_element(int position, int _piece, int _color){
    board[position].piece=_piece;
    board[position].piece_color=_color;
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
        cout<<"["<<board[i].piece<<","<<board[i].piece_color<<","<<board[i].square_color<<"] ";
        count++;
        if (count%8==0){
            cout<<endl;
        
        }
        
    }
    cout<<endl;
}

int Chessboard::find_pawn_in_column(Chessboard input, int column, int color){
    int position =8;
    if (color==1){
        position=-1;
    }
    int col=0;
    for (int i=column; i<63;i+=8){
        if (input.board[i].piece==6){
            if (input.board[i].piece_color==color){
                position = col;
            }
        }
        col+=1;
    }
    return position;
}
