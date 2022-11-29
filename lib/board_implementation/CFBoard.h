#pragma once

class CFBoard {
public:

	CFBoard();
	CFBoard(std::string FEN) : CFBoard() { fromFEN(FEN); } // TO DO
	void fromFEN(std::string FEN); // TO DO

	std::string getRepr();
	std::string toFEN(); // TO DO

	void addPiece(char pieceType, bool color, int tile); // pieceType in P,R,N,B,Q,K and 0<=tile<=63. Function replaces existing piece on the tile.
	void removePiece(int tile);

	// ----- formatting functions -----
	char pieceIdToChar(int pieceId);
	bool pieceIdToColor(int pieceId);
	int pieceCharColorToId(char pieceChar, bool color);

	// ----- get functions -----

	bool getBit(char pieceType, bool color, int tile);

	// these create a copy
	uint64_t getPieceBitBoard(char pieceType);
	uint64_t getColorBitBoard(bool color);

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
	uint64_t PawnBoard;
	uint64_t KingBoard;
	uint64_t QueenBoard;
	uint64_t RookBoard;
	uint64_t KnightBoard;
	uint64_t BishopBoard;

	uint64_t BlackBoard;
	uint64_t WhiteBoard;

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