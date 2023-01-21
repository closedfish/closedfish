#pragma once

#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <string.h>

#include <CFBoard.h>
#include "BitOperations.h"

namespace Heatmap {

	/**
	* @brief This function takes a boord coordinate and returns whether it is within the board.
	*
	* @param i : <int> the row of the tile
	* @param j : <int> the column of the tile
	*
	* @return true or false
	*/
	bool validSquare(int i, int j);


	/**
	* @brief This function takes a 0-63 integer and returns the corresponding coordinate tuple.
	*
	* @param tile : <int> the 0-63 integer corresponding to a bitboard index
	*
	* @return std::vector<int> where the 0th element is the row, and the 1st element is the column.
	*/
	std::vector<int> tileToPos(int tile);


	/**
	* @brief This function converts a coordinate vector<int> to a bitboard position index 0-63.
	*
	* @param pos : std::vector<int> where the 0th element is the row, and the 1st element is the column.
	*
	* @return  <int> the 0-63 integer corresponding to a bitboard index
	*/
	int posToTile(std::vector<int> pos);


	/**
	*@brief This function takes a bit board and returns a vector containing the position tuples of all the 1s on it.
	*
	*
	* @param board: a <uint64_t> bitboard passed by reference.
	* 
	* @return a list of tuples of the form (rowNum,columnNum).
	*/
	std::vector<std::vector<int>> posSetFromBoard(const uint64_t& board);


	/**
	* @brief This function returns a bool of whether the piece at the given position is being attacked by an opponent's pawn.
	*/
	bool squareNotAttackedByPawn(const uint64_t& opponentPawnBoard,
		const int& row, const int& col);


	/**
	* @brief Debug function to visualize a pawn board
	*/
	void displayPawnBoard(const uint64_t& pawnBoard);

	void addHeatMapPieceProtect(const int& i, const int& j, int(&heatMap)[6][8][8], const char& piece,
		const int& noPieces, const uint64_t& opponentPawnBoard, int(&pawnHeight)[8], const int& coefficient = 1);


	/**
	* @brief The main function that takes an int[6][8][8] by reference, and stores the heatmap there.
	*
	* @param board : the current board instance.
	* @param heatMap : the variable that the heatmap will be stored in. 
	* @param weakPawns : the bitboard obtained by running weak pawns.
	*
	*/
	void addHeatMap(CFBoard& board, int(&heatMap)[6][8][8], const uint64_t& weakPawns);

}

