//
//  generator.hpp
//  Generator
//
//  Created by Ariel Flahaut on 23/11/2022.
//

#ifndef generator_hpp
#define generator_hpp

#include <stdio.h>

#endif /* generator_hpp */
Chessboard single_generator(int pawns); //Input number of pawns and return a random closed position with only pawns

void file_generator(int pawns); //Input number of pawns and return a file with every possible closed position with only pawns

void database_edit(); //Etienne, open the database file, remove all of the unneeded pieces (not pawns) and create a new file with all of the new positions that have only pawns.

std::array<int, 64> completion(std::array<int, 64>);// Shirelle, input a board with only pawns and fill the board with other pieces randomly


double closeness(std::array<int,64>); //input a position consiting only of pawns, return a percentage

double openness(std::array<int,64>); //input a position consiting only of pawns, return a percentage



class ArrayElement { // Array element class
public:
    ArrayElement(){
        
    }
    ArrayElement(int piece, int piece_color, int square_color){ // constructor
        this->piece = piece;
        this->piece_color = piece_color;
        this->square_color = square_color;
    }
    
    bool is_empty(){ // Checks if the position is empty
        if (piece == 0){
            return true;
        }
        else {
            return false;
        }
    }
    
    // Grabs private values
    double get_piece(){
        return piece;
    }
    double get_piece_color(){
        return piece_color;
    }
    double get_square_color(){
        return square_color;
    }
        
private:
    int piece;
    int piece_color;
    int square_color;
};

/*Notation: Chess board representation

 An array of 64 elements, with each element of the array from a class, with attributes "piece" indicating the piece on said square , "piece_color" indicating the color of the piece, and "square_color" indicating the color of the square.
 
 The pieces shall be named: king = 1, queen = 2, bishop = 3, knight = 4, rook (castle) = 5, pawn = 6
 
 For the color of the pieces: white = 0, black = 1
 
 For the color of the square: white = 0, black = 1
 
 For example: The first row of an untouched chess game would be:
 {elem1, elem2, elem3, elem4, elem5, elem6, elem7, elem8, ... }
 
 where the first piece, (a black rook in the upper left corner);
 elem1.get_piece() = 5 (a rook)
 elem1.get_piece_color() = 1 (a black piece)
 elem1.get_square_color() = 0 (a white square)

*/

class Chessboard{
public:
    Chessboard();
    void replace_element(int, ArrayElement);
    ArrayElement board[64];

}
