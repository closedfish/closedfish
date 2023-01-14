#pragma once
#define DEBUG 1 //turn off for final version

#ifdef _MSC_VER
#include <nmmintrin.h>
#include <immintrin.h>
#define __builtin_popcountll _mm_popcnt_u64
#define __builtin_ctzll _tzcnt_u64
#define __builtin_clzll _lzcnt_u64
#endif

#include <iostream>
#include <stdint.h>


class CFBoard {
public:

	// ----- Constructors, Formatting, Representation -----

	CFBoard();

	CFBoard(std::string FEN) : CFBoard() { fromFEN(FEN); }
	CFBoard(uint64_t pawnBoard, uint64_t knightBoard, uint64_t bishopBoard,
		uint64_t rookBoard, uint64_t queenBoard, uint64_t kingBoard,
		int enPassantTarget, int castleCheck, uint64_t blackBoard,
		uint64_t whiteBoard, bool turn)
		: pawnBoard(pawnBoard), knightBoard(knightBoard),
		bishopBoard(bishopBoard), rookBoard(rookBoard),
		queenBoard(queenBoard), kingBoard(kingBoard),
		enPassantTarget(enPassantTarget), castleCheck(castleCheck),
		blackBoard(blackBoard), whiteBoard(whiteBoard), turn(turn) {}

	void fromFEN(std::string FEN); // TO DO
	std::string toFEN();


	/**
	* @brief Returns printable board representation.
	*/
	std::string getRepr();
	std::string getReprLegalMove(int pieceId, int tile);

	/**
	* @brief This function takes a pieceId and returns the associated
	* character.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the
	* piece is black.
	*
	* @return P/N/B/R/Q/K depending on the piece, lowercase if black piece.
	*/
	char pieceIdToChar(int pieceId);

	/**
	* @brief This function takes a pieceId and returns the associated
	* character.
	*
	* @param pieceChar : <char> P/N/B/R/Q/K depending on the piece, lowercase
	* if black piece.
	*
	* @return <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is
	* black.
	*/
	int pieceCharToId(char pieceChar);



	// ----- Get functions -----


	/**
	* @brief This function returns the bitboard for one of the two chess piece
	* colors.
	*
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> copy of stored attribute for all pieces of a color.
	*/
	uint64_t getColorBitBoard(bool color);


	/**
	* @brief This function returns a bitboard for a piece of a specific color.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the
	* piece is black.
	*
	* @return <uint64_t> bitboard for the specified piece (color taken into
	* account).
	*/
	uint64_t getPieceColorBitBoard(int pieceId);


	/**
	* @brief This function returns a reference to a board, in order to make
	* iterating over boards easier.
	*
	* @param boardIndex : Index of the required piece bitboard (identical to
	* pieceId>>1).
	*
	* @return <uint64_t &> reference to private attribute bitboard.
	*/
	uint64_t& getPieceBoardFromIndex(int boardIndex);


	/**
	* @brief Returns whose turn it is to play.
	*
	* @return Material count for that color.
	*/
	bool getCurrentPlayer();


	/**
	* @brief This function returns the piece id from a specific tile.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	*
	* @return <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is
	* black.
	*/
	int getPieceFromCoords(int tile);


	/**
	* @brief This function returns whether or not there is a piece of pieceId
	* in the specified tile.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the
	* piece is black.
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	*
	* @return <bool> 1 if the piece is on that tile, 0 otherwise.
	*/
	bool getBit(int pieceId, int tile);


	/**
	* @brief Computes the material count for a specific color.
	*
	* @param color : 1 for black, 0 for white.
	*
	* @return Material count for that color.
	*/
	int getMaterialCount(bool color);

	std::string tileToCoords(int tile);
	std::string getNextMoveRepr(int startTile, int endTile);

	// ----- Manipulation -----

	/**
	* @brief Only accepts legal chess moves. Pawns are promoted to queens by default.
	*
	* @param startTile : start tile for move.
	* @param endTile : end tile for move.
	* @param pawnPromotionType : one of 2/4/6/8 (+1 if black) => N/B/R/Q which indicates the type to which the pawn is promoted in the event of a pawn promotion move.
	*
	* @return void.
	*/
	void movePiece(int starttile, int endtile, int pawnPromotionType = -1);


	/**
	* @brief Functionnaly the same as movePiece. Accepts every single possible move however, and makes the state illegal from now until this move is undone using undoLastMove.
	*
	* @param startTile : start tile for move.
	* @param endTile : end tile for move.
	* @param pawnPromotionType : one of 2/4/6/8 (+1 if black) => N/B/R/Q which indicates the type to which the pawn is promoted in the event of a pawn promotion move.
	*
	* @return void.
	*/
	void forceMovePiece(int starttile, int endtile, int pawnPromotionType = -1);




	/**
	* @brief Undoes the last move exactly using our state backup (can only be done 4 times in a row max currently)
	*
	* @return void.
	*/
	void undoLastMove();


	// -------   Debug-only Manipulation -------

#if DEBUG == 1
	/**
	* @brief Functionally the same as addPiece but is only exposed when debug is enabled
	*/
	void forceAddPiece(int pieceId, int tile) {
		addPiece(pieceId, tile);
	}

	/**
	* @brief Functionally the same as removePiece but is only exposed when debug is enabled
	*/
	void forceRemovePiece(int tile) {
		removePiece(tile);
	}
#endif

	// ----- Ruleset -----

	/**
	* @brief This function returns the naive move pattern for a rook.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a rook at tile can move/capture.
	*/
	uint64_t getCardinals(int tile, bool color);


	/**
	* @brief This function returns the naive move pattern for a bishop.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a bishop at tile can move/capture.
	*/
	uint64_t getDiagonals(int tile, bool color);


	/**
	* @brief This function returns the naive move pattern for a knight.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a knight at tile can move/capture.
	*/
	uint64_t getKnightPattern(int tile, bool color);

	/**
	* @brief This function returns the naive move pattern for a king.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a king at tile can move/capture.
	*/
	uint64_t getKingPattern(int tile, bool color);


	/**
	* @brief This function returns the naive move pattern for a pawn.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	* @param color : 1 for black, 0 for white.
	*
	* @return <uint64_t> bitboard for where a pawn at tile can move/capture.
	*/
	uint64_t getPawnPattern(int tile, bool color);


	/**
	* @brief This function returns the naive move pattern for a pieceId piece.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the
	* piece is black.
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	*
	* @return <uint64_t> bitboard for where a piece of pieceId at tile can
	* move/capture.
	*/
	uint64_t getLegalMoves(int pieceId, int tile);


	/**
	* @brief As soon as a forced move is performed, we return false even if the current board could be legal. Basically a check of whether the current board was only reached using fully legal moves.

	* @return The boolean which indicates legality of the board.
	*/
	bool isCurrentBoardLegal() {
		return isStateLegal;
	}


	/**
	 * @brief Checks if the current king is being checked by the opponent or not
	 *
	 * @param color current color
	 * @param coordA (optional) pretend that there is nothing here
	 * @param coordB (optional) pretend that there is something here with the
	 * current color
	 * @return true if it is checked
	 * @return false if it is not checked
	 */
	bool naiveCheckCheck(bool color, int coordA = -1, int coordB = -1);

	// Misc

	friend bool operator==(const CFBoard& board1, const CFBoard& board2) {
		return board1.pawnBoard == board2.pawnBoard &&
			board1.knightBoard == board2.knightBoard &&
			board1.bishopBoard == board2.bishopBoard &&
			board1.rookBoard == board2.rookBoard &&
			board1.queenBoard == board2.queenBoard &&
			board1.kingBoard == board2.kingBoard &&
			board1.enPassantTarget == board2.enPassantTarget &&
			board1.castleCheck == board2.castleCheck &&
			board1.blackBoard == board2.blackBoard &&
			board1.whiteBoard == board2.whiteBoard &&
			board1.turn == board2.turn;
	}

private:

	//----- THE CURRENT VALUES-----
	uint64_t pawnBoard;
	uint64_t knightBoard;
	uint64_t bishopBoard;
	uint64_t rookBoard;
	uint64_t queenBoard;
	uint64_t kingBoard;

	uint64_t blackBoard;
	uint64_t whiteBoard;

	int enPassantTarget; // a single coordinate from 0-63
	int castleCheck; // 4 bits of information
					 //(long black - short black- long white - short white)



	bool turn; // 0 for white, 1 for black
	bool isStateLegal = true;//tells us whether the current state is legal i.e. no forced manipulations were done



	//--------THE BACKUP OF VALUES FROM PREVIOUS STATES
	const static int backupCount = 4;
	int backupStock = 0; //how many backups we have in stock

	uint64_t* pawnBoardBackups;
	uint64_t* knightBoardBackups;
	uint64_t* bishopBoardBackups;
	uint64_t* rookBoardBackups;
	uint64_t* queenBoardBackups;
	uint64_t* kingBoardBackups;

	uint64_t* blackBoardBackups;
	uint64_t* whiteBoardBackups;

	int* enPassantTargetBackups; 
	int* castleCheckBackups; 

	bool* isStateLegalBackups; //cursed data type : please fix if you're reading this

	/**
	* @brief This function places a piece on a given tile. It will replace any
	* piece on the target tile.
	*
	* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the
	* piece is black.
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	*
	* @return void
	*/
	void addPiece(int pieceId, int tile);


	/**
	* @brief This function removes a piece on a given tile.
	*
	* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
	* h7, ......, a1, ..., h1).
	*
	* @return void
	*/
	void removePiece(int tile);

	/**
	* @brief This function backs up the current state for rolling back and undoing moves later
	* @return void
	*/
	void backupState();
};
