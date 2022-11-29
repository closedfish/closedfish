#pragma once

class CFBoard {
public:

	CFBoard();

	void addPiece(char pieceType, bool color, int tile); // pieceType in P,R,N,B,Q,K and 0<=tile<=63. Function replaces existing piece on the tile.
	void removePiece(int tile);
	//void naiveMovePiece(int starttile, int endtile);

	bool getBit(char pieceType, bool color, int tile);



	/**
	* @brief Returns a copy of the bitboard associated to the input piece type.
	*
	* @param pieceType : a character like "P", "K", "N"...
	*
	* @return an unsigned 64 bit int with a binary representation corresponding to the bitboard.
	*/
	uint64_t getPieceBitBoard(char pieceType);




	uint64_t getColorBitBoard(bool color);

	std::string getRepr();

	// Sirawit, 27/11/2022: I'm not sure we're going to implement this or not, but it would be really good for the switch part if we can do this
	CFBoard(std::string FEN) : CFBoard() { fromFEN(FEN); }
	void fromFEN(std::string FEN);
	std::string toFEN();
	// End (Sirawit, 27/11/2022)


	/*
	uint64_t getCardinals(int tile, bool color);
	uint64_t getDiagonals(int tile, bool color);

	uint64_t getNaiveLegalMoves(); // What should input be here?

	bool checkCheck();
	bool checkCheckmate(); // maybe?

	void move();

	*/

private:

	uint64_t pawnBoard;
	uint64_t kingBoard;
	uint64_t queenBoard;
	uint64_t rookBoard;
	uint64_t knightBoard;
	uint64_t bishopBoard;


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