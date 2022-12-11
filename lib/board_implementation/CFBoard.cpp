#include "CFBoard.h"
#include <bitset>
#include <cctype>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

#include "naiveCheckCheck.cpp"

// implemented

/*
int main(){
    //CFBoard testBoard = CFBoard();
    CFBoard testBoard = CFBoard("8/8/8/8/8/8/8/8 w - -");

    std::cout << "5" << std::endl;

    for (int i=0; i<12; i++){
        std::cout << testBoard.getReprLegalMove(i, 36) << std::endl;
    }
    
    return 0;
}
*/

// ----- Constructors, Formatting, Representation -----

CFBoard::CFBoard() { // This is just the starter board.
    pawnBoard = (((1ll << 8) - 1) << 48) + (((1ll << 8) - 1) << 8);
    knightBoard = (1ll << 1) + (1ll << 6) + (1ll << 57) + (1ll << 62);
    bishopBoard = (1ll << 2) + (1ll << 5) + (1ll << 58) + (1ll << 61);
    rookBoard = (1ll << 0) + (1ll << 7) + (1ll << 56) + (1ll << 63);
    queenBoard = (1ll << 3) + (1ll << 59);
    kingBoard = (1ll << 4) + (1ll << 60);

    whiteBoard = ((1ll << 16) - 1) << 48;
    blackBoard = (1ll << 16) - 1;

    turn = 0;
    enPassantTarget = -1;
    castleCheck = 15;
}

void CFBoard::fromFEN(std::string FEN) {
    // configure the current board from input FEN
    // Clearing the board
    pawnBoard = 0LL;
    kingBoard = 0LL;
    queenBoard = 0LL;
    rookBoard = 0LL;
    knightBoard = 0LL;
    bishopBoard = 0LL;
    blackBoard = 0LL;
    whiteBoard = 0LL;
    turn = 0;
    castleCheck = 0;
    enPassantTarget = -1;

    // split a string python-like
    auto split = [](const std::string &text, char sep) {
        std::vector<std::string> tokens;
        std::size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens;
    };

    // Vector of fields of FEN, index 0: board, 1: current turn, 2: castling, 3:
    // e.p., 4: half-move clock, 5: full-move counter
    std::vector<std::string> fields = split(FEN, ' ');

    // Board
    std::string string_board = fields[0];
    std::vector<std::string> rows = split(string_board, '/');

    for (int row_index = 0; row_index < 8; row_index++) {
        int col_index = 0;
        for (auto ch : rows[row_index]) {
            // ch can be digit meaning consecutive empty squares in a row,
            // or characters representing a piece (upper case if white pieces)
            if (isdigit(ch)) {
                col_index += ch - '0';
            } else {
                int pieceId = pieceCharToId(ch);
                this->addPiece(pieceId, row_index * 8 + col_index);
                col_index++;
            }
        }
    }

    // Current turn
    std::string string_current_turn = fields[1];
    turn = (string_current_turn == "b");

    // Castling
    std::string string_castling = fields[2];
    if (string_castling != "-") {
        // dependent on how you want to store castling availability
        // temporarily I use the first four bits of castleBools for K, Q, k, q
        // respectively
        if (string_castling.find('K') != std::string::npos) {
            castleCheck |= 1;
        }
        if (string_castling.find('Q') != std::string::npos) {
            castleCheck |= 2;
        }
        if (string_castling.find('k') != std::string::npos) {
            castleCheck |= 4;
        }
        if (string_castling.find('q') != std::string::npos) {
            castleCheck |= 8;
        }
    }

    // En passant
    std::string string_enpassant = fields[3];
    if (string_enpassant != "-") {
        // 16 bits used for squares a6->h6, then a3->h3
        int col = string_enpassant[0] - 'a';
        int row = string_enpassant[1] - '1';
        enPassantTarget = row * 8 + col;
    }
}

std::string CFBoard::toFEN() {
    std::string fenString = "";
    // 1. piece placement data
    for (int row = 0; row < 8; row++) {
        std::string currentRow = "";
        int emptyStreak = 0;
        for (int col = 0; col < 8; col++) {
            uint64_t bit = 1ll << (row * 8ll + col);
            if ((whiteBoard | blackBoard) & bit) {
                if (emptyStreak) {
                    currentRow.push_back(emptyStreak + '0');
                    emptyStreak = 0;
                }
                int pieceCode = getPieceFromCoords(row * 8 + col);
                char pieceChar = pieceIdToChar(pieceCode);
                if (blackBoard & bit)
                    pieceChar = tolower(pieceChar);
                currentRow.push_back(pieceChar);
            } else {
                emptyStreak++;
            }
        }
        if (emptyStreak) {
            currentRow.push_back(emptyStreak + '0');
            emptyStreak = 0;
        }
        if (row)
            fenString.push_back('/');
        fenString += currentRow;
    }
    fenString.push_back(' ');
    // 2. active color
    if (turn)
        fenString.push_back('b');
    else
        fenString.push_back('w');
    // 3. castling
    fenString.push_back(' ');
    if (castleCheck & 1)
        fenString.push_back('K');
    if (castleCheck & 2)
        fenString.push_back('Q');
    if (castleCheck & 4)
        fenString.push_back('k');
    if (castleCheck & 8)
        fenString.push_back('q');
    if ((castleCheck & 15) == 0)
        fenString.push_back('-');
    // 4. en passant
    fenString.push_back(' ');
    if (enPassantTarget == -1) {
        fenString.push_back('-');
    } else {
        int row = enPassantTarget >> 3;
        int col = enPassantTarget & 7;
        fenString.push_back(col + 'a');
        fenString.push_back(row + '1');
    }
    // (optional: not that important)
    // 5. halfmove clock
    fenString += " 0";
    // 6. fullmove number
    fenString += " 1";
    return fenString;
}

/**
* @brief Returns printable board representation.
*/
std::string CFBoard::getRepr() {
    std::string repr = "|";
    for (int tile = 0; tile < 64; tile++) {
        repr += " ";

        int pieceId = getPieceFromCoords(tile);
        char pieceChar = pieceIdToChar(pieceId);
        repr += pieceChar;

        repr += " |";
        if ((tile + 1) % 8 == 0) {
            repr += "\n|";
        }
    }
    return repr;
}

std::string CFBoard::getReprLegalMove(int pieceId, int tile){
    uint64_t legalMoves = getLegalMoves(pieceId, tile);
    std::string repr = "|";
    bool isLegalMove;
    for (int tileI = 0; tileI < 64; tileI++) {
        isLegalMove = (legalMoves>>tileI)&1;
        repr += isLegalMove?"[":(tileI == tile)?">":" ";

        int pieceIdI = getPieceFromCoords(tileI);
        char pieceCharI = pieceIdToChar(pieceIdI);
        repr += pieceCharI;

        repr += isLegalMove?"]":(tileI == tile)?"<":" ";
        repr += "|";
        if ((tileI + 1) % 8 == 0) {
            repr += "\n|";
        }
    }
    return repr;
}

/**
* @brief This function takes a pieceId and returns the associated
* character.
*
* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the
* piece is black.
*
* @return P/N/B/R/Q/K depending on the piece, lowercase if black piece.
*/
char CFBoard::pieceIdToChar(int pieceId) {
    char pieceChar = '.';
    bool color = pieceId & 1;
    pieceId = pieceId >> 1;
    switch (pieceId) {
    case 0:
        pieceChar = 'P';
        break;
    case 1:
        pieceChar = 'N';
        break;
    case 2:
        pieceChar = 'B';
        break;
    case 3:
        pieceChar = 'R';
        break;
    case 4:
        pieceChar = 'Q';
        break;
    case 5:
        pieceChar = 'K';
        break;
    default:
        break;
    }
    if (color) {
        pieceChar = tolower(pieceChar);
    }
    return pieceChar;
}

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
int CFBoard::pieceCharToId(char pieceChar) {
    int pieceId = -1;
    switch (pieceChar) {
    case 'P':
        return 0;
    case 'p':
        return 1;
    case 'N':
        return 2;
    case 'n':
        return 3;
    case 'B':
        return 4;
    case 'b':
        return 5;
    case 'R':
        return 6;
    case 'r':
        return 7;
    case 'Q':
        return 8;
    case 'q':
        return 9;
    case 'K':
        return 10;
    case 'k':
        return 11;
    }
    return pieceId;
}

// ----- Get functions -----

/**
* @brief This function returns the bitboard for one of the two chess piece
* colors.
*
* @param color : 1 for black, 0 for white.
*
* @return <uint64_t> copy of stored attribute for all pieces of a color.
*/
uint64_t CFBoard::getColorBitBoard(bool color) {
    if (color) {
        return blackBoard;
    }
    return whiteBoard;
}

/**
* @brief This function returns a bitboard for a piece of a specific color.
*
* @param pieceId : <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the
* piece is black.
*
* @return <uint64_t> bitboard for the specified piece (color taken into
* account).
*/
uint64_t CFBoard::getPieceColorBitBoard(int pieceId) {
    return getPieceBoardFromIndex(pieceId >> 1) & getColorBitBoard(pieceId & 1);
}

/**
* @brief This function returns a reference to a board, in order to make
* iterating over boards easier.
*
* @param boardIndex : Index of the required piece bitboard (identical to
* pieceId>>1).
*
* @return <uint64_t &> reference to private attribute bitboard.
*/
uint64_t &CFBoard::getPieceBoardFromIndex(int boardIndex) {
    switch (boardIndex) {
    case 0:
        return pawnBoard;
    case 1:
        return knightBoard;
    case 2:
        return bishopBoard;
    case 3:
        return rookBoard;
    case 4:
        return queenBoard;
    default:
        return kingBoard;
    }
}

/**
* @brief Returns whose turn it is to play.
*
* @return Material count for that color.
*/
bool CFBoard::getCurrentPlayer() { return turn; }

/**
* @brief This function returns the piece id from a specific tile.
*
* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
* h7, ......, a1, ..., h1).
*
* @return <int> equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is
* black.
*/
int CFBoard::getPieceFromCoords(int tile) {
    for (int i = 0; i < 6; i++) {
        if ((getPieceBoardFromIndex(i) >> tile) & 1) {
            return (i << 1) | ((blackBoard >> tile) & 1);
        }
    }
    return -1;
}

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
bool CFBoard::getBit(int pieceId, int tile) {
    return (getPieceColorBitBoard(pieceId) >> tile) & 1;
}

/**
* @brief Computes the material count for a specific color.
*
* @param color : 1 for black, 0 for white.
*
* @return Material count for that color.
*/
int CFBoard::getMaterialCount(bool color) {
    return __builtin_popcountll(pawnBoard) * 1 +
           __builtin_popcountll(knightBoard) * 3 +
           __builtin_popcountll(bishopBoard) * 3 +
           __builtin_popcountll(rookBoard) * 5 +
           __builtin_popcountll(queenBoard) * 9;
}

/**
* @brief Gives a text representation of a coordinate.
*
* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
* h7, ......, a1, ..., h1).
* 
* @return string representation of the corresponding board tile coordinate
*/
std::string CFBoard::tileToCoords(int tile){
    std::string ret = "";
    
    int column = tile & 7;
    int row = tile >> 3;

    ret += 97 + row;
    ret += column + 1;

    return ret;
}

/**
* @brief Gives a text representation of a hypothetical move.
*
* @param startTile : start tile for move.
* @param endTile : end tile for move.
* 
* @return string representation of the move from startTile to endTile
*/
std::string CFBoard::getNextMoveRepr(int startTile, int endTile){

    int piece = getPieceFromCoords(startTile);

    if ((piece>>1 == 5) && (abs(startTile - endTile)==2)){
        int castle;
        if (abs(startTile - endTile)==2){
            return "O-O-O";
        } else {
            return "O-O";
        }

    }

    std::string ret = "";

    ret += pieceIdToChar(piece);
    if (getPieceFromCoords(endTile) != -1){
        ret += "x";
    }

    ret += tileToCoords(startTile);
    ret += tileToCoords(endTile);

    return ret;
}

// ----- Manipulation -----
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
void CFBoard::addPiece(int pieceId, int tile) {
    uint64_t pieceBoard = 1ll << tile;
    removePiece(tile);

    int pieceType = pieceId >> 1;
    bool color = pieceId & 1;

    uint64_t &targetBoard = getPieceBoardFromIndex(pieceType);
    targetBoard = targetBoard | pieceBoard;

    if (color) {
        blackBoard = blackBoard | pieceBoard;
    } else {
        whiteBoard = whiteBoard | pieceBoard;
    }
}

/**
* @brief This function removes a piece on a given tile.
*
* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
* h7, ......, a1, ..., h1).
*
* @return void
*/
void CFBoard::removePiece(int tile) {
    uint64_t antiPieceBoard = ~(1ll << tile);
    for (int pieceType = 0; pieceType < 6; pieceType++) {
        uint64_t &targetBoard = getPieceBoardFromIndex(pieceType);
        targetBoard = targetBoard & antiPieceBoard;
    }
    blackBoard = blackBoard & antiPieceBoard;
    whiteBoard = whiteBoard & antiPieceBoard;
}

/**
* @brief Makes a legal chess move.
*
* @param startTile : start tile for move.
* @param endTile : end tile for move.
* 
* @return void.
*/
void CFBoard::movePiece(int startTile, int endTile){
		int piece = getPieceFromCoords(startTile);
		if (~((1ll << endTile) & getLegalMoves(pieceIdToChar(startTile), startTile))){
			exit(-1);
		}
		if ((piece & 1) ^ turn){
			exit(-1);
		}

		removePiece(startTile);
		addPiece(piece, endTile);

		if (~turn){ // white
			if ((piece>>1) == 3){ // rook
				if (startTile == 63){
					castleCheck = castleCheck & ~1;
				} else if (startTile == 56){
                    castleCheck = castleCheck & ~2;
                }
			}
            if ((piece>>1) == 5){ // king
                castleCheck = castleCheck & ~3;
            }

            if ((piece>>1) == 0){ // pawn
                if ((startTile - endTile) == 16){
                    enPassantTarget = startTile - 8;
                }
            }
		} else {
            if ((piece>>1) == 3){ // rook
				if (startTile == 0){
					castleCheck = castleCheck & ~8;
				} else if (startTile == 7){
                    castleCheck = castleCheck & ~4;
                }
			}
            if ((piece>>1) == 5){ // king
                castleCheck = castleCheck & ~12;
            }

            if ((piece>>1) == 0){ // pawn
                if ((endTile - startTile) == 16){
                    enPassantTarget = startTile + 8;
                }
            }
        }

        if ((piece>>1 == 5) && (abs(startTile - endTile)==2)){
            int castle;
            if (piece&1){
                castle = castleCheck >> 2;
            } else {
                castle = castleCheck & 3;
            }
            castleCheck = castleCheck & ~castle;
            if (abs(startTile - endTile)==2){
                removePiece(startTile - 4);
                addPiece(6 + (piece & 1), startTile - 1);
            } else {
                removePiece(startTile + 3);
                addPiece(6 + (piece & 1), startTile + 1);
            }

        }

		turn = ~turn;
	}

/**
* @brief Forces the undo of a move. Makes assumptions on castling and en passant (they do not get updated).
*
* @param startTileLastTurn : start tile for last move.
* @param endTileLastTurn : end tile for last move.
* @param capturedPiece : pieceId for any potentially captured piece last turn. -1 (no piece) by default.
* 
* @return void.
*/
void CFBoard::forceUndo(int startTileLastTurn, int endTileLastTurn, int capturedPiece = -1){
    int piece = getPieceFromCoords(endTileLastTurn);
    if (capturedPiece == -1){
        removePiece(endTileLastTurn);
    } else {
        addPiece(capturedPiece, endTileLastTurn);
    }
    addPiece(piece, startTileLastTurn);
    enPassantTarget = -1;
}

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
uint64_t CFBoard::getCardinals(int tile, bool color) {
    uint64_t cardinalBoard = 0;
    uint64_t tempBoard = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);
    uint64_t enemyBoard = getColorBitBoard(!color);

    // left
    for (int i = 1; i <= column; i++) {
        tempBoard = (1ll << (tile - i));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            cardinalBoard = cardinalBoard | tempBoard;
            break;
        }
        cardinalBoard = cardinalBoard | tempBoard;
    }

    // right
    for (int i = 1; i <= (7 - column); i++) {
        tempBoard = (1ll << (tile + i));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            cardinalBoard = cardinalBoard | tempBoard;
            break;
        }
        cardinalBoard = cardinalBoard | tempBoard;
    }

    // up
    for (int i = 1; i <= row; i++) {
        tempBoard = (1ll << (tile - 8 * i));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            cardinalBoard = cardinalBoard | tempBoard;
            break;
        }
        cardinalBoard = cardinalBoard | tempBoard;
    }

    // down
    for (int i = 1; i <= (7 - row); i++) {
        tempBoard = (1ll << (tile + 8 * i));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            cardinalBoard = cardinalBoard | tempBoard;
            break;
        }
        cardinalBoard = cardinalBoard | tempBoard;
    }

    return cardinalBoard;
}

/**
* @brief This function returns the naive move pattern for a bishop.
*
* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
* h7, ......, a1, ..., h1).
* @param color : 1 for black, 0 for white.
*
* @return <uint64_t> bitboard for where a bishop at tile can move/capture.
*/
uint64_t CFBoard::getDiagonals(int tile, bool color) {
    uint64_t diagonalBoard = 0;
    uint64_t tempBoard = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);
    uint64_t enemyBoard = getColorBitBoard(!color);

    // up-left
    for (int i = 1; i <= (column ^ ((row ^ column) & -(row < column)));
         i++) { // y ^ ((x ^ y) & -(x < y)) = min(x,y)
        tempBoard = (1ll << (tile - (9 * i)));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            diagonalBoard = diagonalBoard | tempBoard;
            break;
        }
        diagonalBoard = diagonalBoard | tempBoard;
    }

    // up-right
    for (int i = 1;
         i <= ((7 - column) ^ ((row ^ (7 - column)) & -(row < (7 - column))));
         i++) {
        tempBoard = (1ll << (tile - (7 * i)));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            diagonalBoard = diagonalBoard | tempBoard;
            break;
        }
        diagonalBoard = diagonalBoard | tempBoard;
    }

    // down-left
    for (int i = 1;
         i <= (column ^ (((7 - row) ^ column) & -((7 - row) < column))); i++) {
        tempBoard = (1ll << (tile + (7 * i)));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            diagonalBoard = diagonalBoard | tempBoard;
            break;
        }
        diagonalBoard = diagonalBoard | tempBoard;
    }

    // down-right
    for (int i = 1; i <= ((7 - column) ^ (((7 - row) ^ (7 - column)) &
                                          -((7 - row) < (7 - column))));
         i++) {
        tempBoard = (1ll << (tile + (9 * i)));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            diagonalBoard = diagonalBoard | tempBoard;
            break;
        }
        diagonalBoard = diagonalBoard | tempBoard;
    }

    return diagonalBoard;
}

/**
* @brief This function returns the naive move pattern for a knight.
*
* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
* h7, ......, a1, ..., h1).
* @param color : 1 for black, 0 for white.
*
* @return <uint64_t> bitboard for where a knight at tile can move/capture.
*/
uint64_t CFBoard::getKnightPattern(int tile, bool color) {
    uint64_t knightPattern = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);

    int knightPositions = (1 << 8) - 1;
    /*
    . 0 . 1 .
    2 . . . 3
    . . x . .
    4 . . . 5
    . 6 . 7 .
    */

    // may be able to be optimized
    switch (column) {
    case 0:
        knightPositions =
            knightPositions & ~((1 << 2) + (1 << 4) + 1 + (1 << 6));
        break;
    case 1:
        knightPositions = knightPositions & ~((1 << 2) + (1 << 4));
        break;
    case 6:
        knightPositions = knightPositions & ~((1 << 3) + (1 << 5));
        break;
    case 7:
        knightPositions =
            knightPositions & ~((1 << 3) + (1 << 5) + 2 + (1 << 7));
        break;
    default:
        break;
    }

    switch (row) {
    case 0:
        knightPositions = knightPositions & ~((1 << 4) - 1);
        break;
    case 1:
        knightPositions = knightPositions & ~((1 << 2) - 1);
        break;
    case 6:
        knightPositions = knightPositions & ~(((1 << 2) - 1) << 6);
        break;
    case 7:
        knightPositions = knightPositions & ~(((1 << 4) - 1) << 4);
        break;
    default:
        break;
    }

    for (int i = 0; i < 8; i++) {
        if ((knightPositions >> i) & 1) {
            switch (i) {
            case 0:
                knightPattern =
                    knightPattern | ((1ll << (tile - 17)) & ~allyBoard);
                break;
            case 1:
                knightPattern =
                    knightPattern | ((1ll << (tile - 15)) & ~allyBoard);
                break;
            case 2:
                knightPattern =
                    knightPattern | ((1ll << (tile - 10)) & ~allyBoard);
                break;
            case 3:
                knightPattern =
                    knightPattern | ((1ll << (tile - 6)) & ~allyBoard);
                break;
            case 4:
                knightPattern =
                    knightPattern | ((1ll << (tile + 6)) & ~allyBoard);
                break;
            case 5:
                knightPattern =
                    knightPattern | ((1ll << (tile + 10)) & ~allyBoard);
                break;
            case 6:
                knightPattern =
                    knightPattern | ((1ll << (tile + 15)) & ~allyBoard);
                break;
            case 7:
                knightPattern =
                    knightPattern | ((1ll << (tile + 17)) & ~allyBoard);
                break;
            }
        }
    }

    return knightPattern;
}

/**
* @brief This function returns the naive move pattern for a king.
*
* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
* h7, ......, a1, ..., h1).
* @param color : 1 for black, 0 for white.
*
* @return <uint64_t> bitboard for where a king at tile can move/capture.
*/
uint64_t CFBoard::getKingPattern(int tile, bool color) {
    uint64_t kingPattern = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);

    if (column > 0) {
        kingPattern += (1ll << (tile - 1));
    }
    if (row > 0) {
        kingPattern += (1ll << (tile - 8));
    }
    if (column < 7) {
        kingPattern += (1ll << (tile + 1));
    }
    if (row < 7) {
        kingPattern += (1ll << (tile + 8));
    }
    if (column > 0 && row > 0) {
        kingPattern += (1ll << (tile - 9));
    }
    if (column > 0 && row < 7) {
        kingPattern += (1ll << (tile + 7));
    }
    if (column < 7 && row > 0) {
        kingPattern += (1ll << (tile - 7));
    }
    if (column < 7 && row < 7) {
        kingPattern += (1ll << (tile + 9));
    }

    //castle
    int castle;
    if (color){
        castle = castleCheck >> 2;
    } else {
        castle = castleCheck & 3;
    }

    // WARNING: this assumes the king is in the right position to castle.
    // If you customized the whole board into an illegal position, this part may crash the code.
    uint64_t board = whiteBoard | blackBoard;
    if (castle>>1){ //long
        bool longsideoccupied = ((board << (tile - 1)) & 1) | ((board << (tile - 2)) & 1) | ((board << (tile - 3)) & 1);
        if (!longsideoccupied){kingPattern += (1ll << (tile-2));}
    }
    if (castle&1){ //short
        bool shortsideoccupied = ((board << (tile + 1)) & 1) | ((board << (tile + 2)) & 1);
        if (!shortsideoccupied){kingPattern += (1ll << (tile+2));}
    }

    return kingPattern;
}

/**
* @brief This function returns the naive move pattern for a pawn.
*
* @param tile : <int> from 0 to 63, in the order (a8, b8, ..., h8, a7, ...,
* h7, ......, a1, ..., h1).
* @param color : 1 for black, 0 for white.
*
* @return <uint64_t> bitboard for where a pawn at tile can move/capture.
*/
uint64_t CFBoard::getPawnPattern(int tile, bool color) {
    uint64_t pawnPattern = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);
    uint64_t enemyBoard = getColorBitBoard(!color);

    if (color) { // black

        if (row == 7) { // just in case, should be useless in practice
            return pawnPattern;
        } else {
            if (!(((allyBoard | enemyBoard) >> (tile + 8)) & 1)) { // front
                pawnPattern += (1ll << (tile + 8));
                if ((row == 1) && (((allyBoard | enemyBoard) >> (tile + 16)) &
                                   1)) { // frontfront
                    pawnPattern += (1ll << (tile + 16));
                }
            }
            // diagonals
            if (column > 0) {
                if ((enPassantTarget == tile + 7) ||
                    (enemyBoard >> (tile + 7) & 1)) {
                    pawnPattern += (1ll << (tile + 7));
                }
            }
            if (column < 7) {
                if ((enPassantTarget == tile + 9) ||
                    (enemyBoard >> (tile + 9) & 1)) {
                    pawnPattern += (1ll << (tile + 9));
                }
            }
        }

    } else { // white

        if (row == 0) { // just in case, should be useless in practice
            return pawnPattern;
        } else {
            if (!(((allyBoard | enemyBoard) >> (tile - 8)) & 1)) { // front
                pawnPattern += (1ll << (tile - 8));
                if ((row == 6) && (((allyBoard | enemyBoard) >> (tile - 16)) &
                                   1)) { // frontfront
                    pawnPattern += (1ll << (tile - 16));
                }
            }
            // diagonals
            if (column > 0) {
                if ((enPassantTarget == tile - 9) ||
                    (enemyBoard >> (tile - 9) & 1)) {
                    pawnPattern += (1ll << (tile - 9));
                }
            }
            if (column < 7) {
                if ((enPassantTarget == tile - 7) ||
                    (enemyBoard >> (tile - 7) & 1)) {
                    pawnPattern += (1ll << (tile - 7));
                }
            }
        }
    }
    return pawnPattern;
}

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
uint64_t CFBoard::getLegalMoves(int pieceId, int tile) {
    bool color = pieceId & 1;
    uint64_t retBoard;
    std::cout << (pieceId>>1) << std::endl;
    switch (pieceId >> 1) {
    case 0: // pawn
        std::cout << "pawn" << std::endl;
        retBoard = getPawnPattern(tile, color);
        break;
    case 1: // knight
        std::cout << "knight" << std::endl;
        retBoard = getKnightPattern(tile, color);
        break;
    case 2: // bishop
        std::cout << "bishop" << std::endl;
        retBoard = getDiagonals(tile, color);
        break;
    case 3: // rook
        std::cout << "rook" << std::endl;
        retBoard = getCardinals(tile, color);
        break;
    case 4: // queen
        std::cout << "queen" << std::endl;
        retBoard = getDiagonals(tile, color) | getCardinals(tile, color);
        break;
    case 5: // king
        std::cout << "king" << std::endl;
        retBoard = getKingPattern(tile, color);
        break;
    }

    uint64_t tmpBoard = retBoard;
    while (tmpBoard) {
        uint64_t lsb = tmpBoard & -tmpBoard;
        tmpBoard ^= lsb;
        int coordB = 63ll - __builtin_clzll(lsb);
        if (naiveCheckCheck(pieceId & 1, tile, coordB)) {
            retBoard ^= lsb;
        }
    }
    return retBoard;
}
