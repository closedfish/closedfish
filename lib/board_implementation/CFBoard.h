#pragma once

#include <iostream>
#include <stdint.h>

class CFBoard {
public:

	CFBoard();
	CFBoard(std::string FEN) : CFBoard() { fromFEN(FEN); } // TO DO
	void fromFEN(std::string FEN); // TO DO

	std::string getRepr();
	std::string toFEN(); // TO DO

	void addPiece(int pieceId, int tile); // pieceType in P,R,N,B,Q,K and 0<=tile<=63. Function replaces existing piece on the tile.
	void removePiece(int tile);

	// ----- formatting functions -----
	char pieceIdToChar(int pieceId);
	bool pieceIdToColor(int pieceId);
	int pieceCharColorToId(char pieceChar, bool color);


	// ----- get functions -----

	/**
	* obsolete - kept for reference
	*
	* @brief Returns a copy of the bitboard associated to the input piece type.
	*
	* @param pieceType : a character like "P", "K", "N"...
	*
	* @return an unsigned 64 bit int with a binary representation corresponding to the bitboard.
	*/

	uint64_t getColorBitBoard(bool color);

	bool getBit(int pieceId, int tile);

	uint64_t& getPieceBoardFromIndex(int boardIndex);

	// these create a copy

	uint64_t getColorBitBoard(bool color);
	uint64_t getPieceColorBitBoard(int pieceId);

	int getPieceFromCoords(int tile); // output is [0,1,2,3,4,5]+(color)
	

	/*
	void naiveMovePiece(int starttile, int endtile);
	uint64_t getCardinals(int tile, bool color);
	uint64_t getDiagonals(int tile, bool color);

	uint64_t getNaiveLegalMoves(); // What should input be here?

	bool checkCheck();
	bool checkCheckmate(); // maybe?

	void move();

	*/

private:

	uint64_t pawnBoard;
	uint64_t knightBoard;
	uint64_t bishopBoard;
	uint64_t rookBoard;
	uint64_t queenBoard;
	uint64_t kingBoard;

	//TODO reduce both of these to the smallest int types
	int enPassantTarget; //a single coordinate from 0-63 
	int castleCheck; //4 bits of information (long white - short white - long black - short black)

	uint64_t blackBoard;
	uint64_t whiteBoard;

	bool turn; // 0 for white, 1 for black

	/*
		a8 = 2^0
		b8 = 2^1
		...
		h1 = 2^63
	*/
};
