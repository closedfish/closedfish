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

	bool getBit(int pieceId, int tile);

	uint64_t& getPieceBoardFromIndex(int boardIndex);

	uint64_t getCardinals(int tile, bool color);
	uint64_t getDiagonals(int tile, bool color);
	uint64_t getKnightPattern(int tile, bool color);
	uint64_t getKingPattern(int tile, bool color);
	uint64_t getPawnPattern(int tile, bool color);

	// currently doesn't check for check
	uint64_t getLegalMoves(int pieceId, int tile);

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
	bool naiveCheckCheck();

private:
	uint64_t pawnBoard;
	uint64_t kingBoard;
	uint64_t queenBoard;
	uint64_t rookBoard;
	uint64_t knightBoard;
	uint64_t bishopBoard;

	uint64_t blackBoard;
	uint64_t whiteBoard;

	bool turn; // 0 for white, 1 for black

	int castleBools;
	int enPassantTarget;

	/*
		a8 = 2^0
		b8 = 2^1
		...
		h1 = 2^63
	*/
};