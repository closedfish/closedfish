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

	/**
	* @brief This function places a piece on a given tile. It will replace any piece on the target tile.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is black. 
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	*
	* @return void
	*/
	void addPiece(int pieceId, int tile);

	/**
	* @brief This function removes a piece on a given tile.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	*
	* @return void
	*/
	void removePiece(int tile);

	// ----- formatting functions -----

	/**
	* @brief This function takes a pieceId and returns the associated character.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is black. 
	*
	* @return P/N/B/R/Q/K depending on the piece, lowercase if black piece.
	*/
	char pieceIdToChar(int pieceId);

	/**
	* @brief This function takes a pieceId and returns the associated character.
	*
	* @param pieceChar : <char> P/N/B/R/Q/K depending on the piece, lowercase if black piece.
	*
	* @return <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is black.
	*/
	int pieceCharToId(char pieceChar);

	// ----- get functions -----

	/**
	* @brief This function returns the bitboard for one of the two chess piece colors.
	*
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> copy of stored attribute for all pieces of a color.
	*/
	uint64_t getColorBitBoard(bool color);

	/**
	* @brief This function returns whether or not there is a piece of pieceId in the specified tile.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is black.
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	*
	* @return <bool> 1 if the piece is on that tile, 0 otherwise.
	*/
	bool getBit(int pieceId, int tile);

	/**
	* @brief This function returns a reference to a board, in order to make iterating over boards easier.
	*
	* @param boardIndex : Index of the required piece bitboard (identical to pieceId>>1).
	*
	* @return <uint64_t &> reference to private attribute bitboard.
	*/
	uint64_t& getPieceBoardFromIndex(int boardIndex);

	/**
	* @brief This function returns the naive move pattern for a rook.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a rook at tile can move/capture.
	*/
	uint64_t getCardinals(int tile, bool color);

	/**
	* @brief This function returns the naive move pattern for a bishop.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a bishop at tile can move/capture.
	*/
	uint64_t getDiagonals(int tile, bool color);

	/**
	* @brief This function returns the naive move pattern for a knight.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a knight at tile can move/capture.
	*/
	uint64_t getKnightPattern(int tile, bool color);

	/**
	* @brief This function returns the naive move pattern for a king.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a king at tile can move/capture.
	*/
	uint64_t getKingPattern(int tile, bool color);

	/**
	* @brief This function returns the naive move pattern for a pawn.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a pawn at tile can move/capture.
	*/
	uint64_t getPawnPattern(int tile, bool color);

	// currently doesn't check for check
	/**
	* @brief This function returns the naive move pattern for a pieceId piece.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is black.
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	*
	* @return <uint64_t> bitboard for where a piece of pieceId at tile can move/capture.
	*/
	uint64_t getLegalMoves(int pieceId, int tile);

	// these create a copy

	/**
	* @brief This function returns a bitboard for a piece of a specific color.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is black.
	*
	* @return <uint64_t> bitboard for the specified piece (color taken into account).
	*/
	uint64_t getPieceColorBitBoard(int pieceId);

	/**
	* @brief This function returns a bitboard for a piece of a specific color.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ..., h7, ......, a1, ..., h1).
	*
	* @return <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is black.
	*/
	int getPieceFromCoords(int tile);
	
	/**
	* @brief Computes the material count for a specific color.
	*
	* @param color : 1 for black, 0 for white.
	*
	* @return Material count for that color.
	*/
	int getMaterialCount(bool color);

	/**
	* @brief Returns whose turn it is to play.
	*
	* @return Material count for that color.
	*/
	bool getCurrentPlayer();

	/*
	void movePiece(int starttile, int endtile){
		int piece = getPieceFromCoords(starttile)
		if ~((1ll << endtile) & getLegalMoves(pieceIdToChar, starttile)){
			exit(-1);
		}
		if ((piece & 1) ^ turn){
			exit(-1);
		}

		removePiece(starttile);
		addPiece(piece, endtile);

		if (~turn){ // white
			if ((piece>>1) == 3){
				if (starttile == 63){
					castleCheck -= 1;
				} else if 
			}
		}

		



		turn = ~turn;
	}
	*/

	/*
	void naiveMovePiece(int starttile, int endtile);

	void move();

	*/



	bool naiveCheckCheck();

private:

	uint64_t pawnBoard;
	uint64_t knightBoard;
	uint64_t bishopBoard;
	uint64_t rookBoard;
	uint64_t queenBoard;
	uint64_t kingBoard;

	//TODO reduce both of these to the smallest int types
	int enPassantTarget; //a single coordinate from 0-63 
	int castleCheck; //4 bits of information (long black - short black- long white - short white)

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
