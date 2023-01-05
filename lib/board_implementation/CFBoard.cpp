#include "CFBoard.h"
#include <bitset>
#include <cctype>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <cmath>


// implemented

/*
int main(){
    
    
    //CFBoard testBoard = CFBoard("8/8/8/8/8/8/8/8 w - -");
    CFBoard testBoard = CFBoard();

    for (int i=10; i<12; i+=1){
        // std::cout << testBoard.getReprLegalMove(i, 0) << std::endl;
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
        repr += (tileI == tile)?">":isLegalMove?"[":" ";

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


uint64_t CFBoard::getColorBitBoard(bool color) {
    if (color) {
        return blackBoard;
    }
    return whiteBoard;
}


uint64_t CFBoard::getPieceColorBitBoard(int pieceId) {
    return getPieceBoardFromIndex(pieceId >> 1) & getColorBitBoard(pieceId & 1);
}


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


bool CFBoard::getCurrentPlayer() { return turn; }


int CFBoard::getPieceFromCoords(int tile) {
    for (int i = 0; i < 6; i++) {
        if ((getPieceBoardFromIndex(i) >> tile) & 1) {
            return (i << 1) | ((blackBoard >> tile) & 1);
        }
    }
    return -1;
}


bool CFBoard::getBit(int pieceId, int tile) {
    return (getPieceColorBitBoard(pieceId) >> tile) & 1;
}


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
    } else if ((piece>>1 == 0) && (endTile == enPassantTarget)){
        ret += "x";
    }

    ret += tileToCoords(startTile);
    ret += tileToCoords(endTile);

    return ret;
}

// ----- Manipulation -----

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


void CFBoard::removePiece(int tile) {
    uint64_t antiPieceBoard = ~(1ll << tile);
    for (int pieceType = 0; pieceType < 6; pieceType++) {
        uint64_t &targetBoard = getPieceBoardFromIndex(pieceType);
        targetBoard = targetBoard & antiPieceBoard;
    }
    blackBoard = blackBoard & antiPieceBoard;
    whiteBoard = whiteBoard & antiPieceBoard;
}


void CFBoard::movePiece(int startTile, int endTile, int pawnPromotionType){
	int piece = getPieceFromCoords(startTile);

	//check that move is legal
	if (  ((1ll << endTile) & getLegalMoves(piece, startTile)) == 0  ){
        std::cerr<<"illegal move!\n";
		exit(-1);
	}

	if ((piece & 1) ^ turn){
        std::cerr<<"illegal move!\n";
		exit(-1);
	}


	//call force move piece
	forceMovePiece(startTile, endTile, pawnPromotionType);


	//make the move legitimate
	isStateLegal = true;
}


void CFBoard::forceMovePiece(int startTile, int endTile, int pawnPromotionType) {
	int piece = getPieceFromCoords(startTile);

	//make a backup of our state
	backupState();


	removePiece(startTile);


	//check whether the pawn reached point of promotion, if so promote it to specified new piece type
	if (piece <= 1) {
		if (piece == 0) {
			if (endTile <= 7) {

				//default promotion to queen
				if (pawnPromotionType == -1) {
					addPiece(8, endTile);
				}
				else if (pawnPromotionType % 2 == 0 && abs(pawnPromotionType - 5) <= 3) {
					addPiece(pawnPromotionType, endTile);
				}
				else {
					exit(-1);
				}
			}
			else {
				addPiece(piece, endTile);
			}
		}
		else {
			if (endTile >= 56) {

				//default promotion to queen
				if (pawnPromotionType == -1) {
					addPiece(9, endTile);
				}
				else if (pawnPromotionType % 2 == 1 && abs(pawnPromotionType - 6) <= 3) {
					addPiece(pawnPromotionType, endTile);
				}
				else {

					exit(-1);
				}

			}
			else {
				addPiece(piece, endTile);

			}
		}
	}
	else {
		addPiece(piece, endTile);
	}


	if (~turn) { // white
		if ((piece >> 1) == 3) { // rook
			if (startTile == 63) {
				castleCheck &= ~1;
			}
			else if (startTile == 56) {
				castleCheck &= ~2;
			}
		}
		if ((piece >> 1) == 5) { // king
			castleCheck &= ~3;
		}

		if ((piece >> 1) == 0) { // pawn
			if ((startTile - endTile) == 16) {
				enPassantTarget = startTile - 8;
			}
		}
	}
	else {
		if ((piece >> 1) == 3) { // rook
			if (startTile == 0) {
				castleCheck &= ~8;
			}
			else if (startTile == 7) {
				castleCheck &= ~4;
			}
		}
		if ((piece >> 1) == 5) { // king
			castleCheck &= ~12;
		}

		if ((piece >> 1) == 0) { // pawn
			if ((endTile - startTile) == 16) {
				enPassantTarget = startTile + 8;
			}
		}
	}

	if ((piece >> 1 == 5) && (abs(startTile - endTile) == 2)) {
		int castle;
		if (piece & 1) {
			castle = castleCheck >> 2;
		}
		else {
			castle = castleCheck & 3;
		}
		castleCheck = castleCheck & ~castle;
		if (abs(startTile - endTile) == 2) {
			removePiece(startTile - 4);
			addPiece(6 + (piece & 1), startTile - 1);
		}
		else {
			removePiece(startTile + 3);
			addPiece(6 + (piece & 1), startTile + 1);
		}

	}

	turn = ~turn;


	//from now on, our state is illegitimate
	isStateLegal = false;

}


void CFBoard::undoLastMove() {
	if (backupStock == 0) { //check that we even have backups
		exit(-1);
	}


	//if so, set our state
	// std::cout << pawnBoard << std::endl;
	// std::cout << pawnBoardBackups[0] << std::endl;

	pawnBoard = pawnBoardBackups[0];
	knightBoard = knightBoardBackups[0] ;
	bishopBoard = bishopBoardBackups[0];
	rookBoard = rookBoardBackups[0];
	queenBoard = queenBoardBackups[0] ;
	kingBoard  = kingBoardBackups[0];

	blackBoard = blackBoardBackups[0];
	whiteBoard = whiteBoardBackups[0];

	enPassantTarget = enPassantTargetBackups[0];
	castleCheck = castleCheckBackups[0];
	isStateLegal = isStateLegalBackups[0];

	turn = 1-turn;



	//update and remove the backup we just reverted to
	backupStock--;

	for (int i = 0; i <= backupCount - 2; i++) {
		pawnBoardBackups[i] = pawnBoardBackups[i + 1];
		knightBoardBackups[i] = knightBoardBackups[i + 1];
		bishopBoardBackups[i] = bishopBoardBackups[i + 1];
		rookBoardBackups[i] = rookBoardBackups[i + 1];
		queenBoardBackups[i] = queenBoardBackups[i + 1];
		kingBoardBackups[i] = kingBoardBackups[i + 1];

		blackBoardBackups[i] = blackBoardBackups[i + 1];
		whiteBoardBackups[i] = whiteBoardBackups[i + 1];

		enPassantTargetBackups[i] = enPassantTargetBackups[i + 1];
		castleCheckBackups[i] = castleCheckBackups[i + 1];
		isStateLegalBackups[i] = isStateLegalBackups[i + 1];


	}

}

// ----- Ruleset -----


uint64_t CFBoard::getCardinals(int tile, bool color) {
    //int column = tile & 7;
    //int row = tile >> 3;

    uint64_t columnMap = (1ll + (1ll<<8) + (1ll<<16) + (1ll<<24) + (1ll<<32) + (1ll<<40) + (1ll<<48) + (1ll<<56)) << (tile & 7);
    uint64_t rowMap = (((1ll << 8) - 1) << (tile>>3<<3));

    uint64_t allBoard = whiteBoard | blackBoard;

    // least significant bit : (b & -b)
    // most significant bit: (1ll << (63 - __builtin_clzll(b)))
    
    // up | left | right | down
    return (\
    (~((1ll << (63 - __builtin_clzll( ((1ll<<tile)-1) & allBoard & columnMap ))) - 1)) & (columnMap >> (64 - (tile>>3<<3))) | \
    (~((1ll << (63 - __builtin_clzll( ((1ll<<tile)-1) & allBoard))) - 1)) & (rowMap & ((1ll<<tile)-1)) | \
    (tile != 63)*((((allBoard & ~((1ll << (tile+1))-1)) & -(allBoard & ~((1ll << (tile+1))-1))) << 1) -1 \
    & (rowMap & ~((1ll << (tile+1))-1))) | (tile != 63)*\
    ((((allBoard & ~((1ll << (tile+1))-1) & columnMap) & -(allBoard & ~((1ll << (tile+1))-1) & columnMap)) << 1) -1\
    & (columnMap & ~((1ll << (tile+1))-1))) ) & (~getColorBitBoard(color));
}


uint64_t CFBoard::getDiagonals(int tile, bool color) {
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allBoard = whiteBoard | blackBoard;

    int slashId = (tile + row + 1) & 7;
    int bslashId = (tile - row) & 7;
    uint64_t slashMap = ((1ll << (8*0 + ((slashId - 0 - 1)&7)) ) * !((row < slashId)^(0 < slashId))) | \
                        ((1ll << (8*1 + ((slashId - 1 - 1)&7)) ) * !((row < slashId)^(1 < slashId))) | \
                        ((1ll << (8*2 + ((slashId - 2 - 1)&7)) ) * !((row < slashId)^(2 < slashId))) | \
                        ((1ll << (8*3 + ((slashId - 3 - 1)&7)) ) * !((row < slashId)^(3 < slashId))) | \
                        ((1ll << (8*4 + ((slashId - 4 - 1)&7)) ) * !((row < slashId)^(4 < slashId))) | \
                        ((1ll << (8*5 + ((slashId - 5 - 1)&7)) ) * !((row < slashId)^(5 < slashId))) | \
                        ((1ll << (8*6 + ((slashId - 6 - 1)&7)) ) * !((row < slashId)^(6 < slashId))) | \
                        ((1ll << (8*7 + ((slashId - 7 - 1)&7)) ) * !((row < slashId)^(7 < slashId)));

    uint64_t bslashMap =((1ll << (8*0 + ((bslashId + 0)&7)) ) * !((7-row < bslashId)^(7-0 < bslashId))) | \
                        ((1ll << (8*1 + ((bslashId + 1)&7)) ) * !((7-row < bslashId)^(7-1 < bslashId))) | \
                        ((1ll << (8*2 + ((bslashId + 2)&7)) ) * !((7-row < bslashId)^(7-2 < bslashId))) | \
                        ((1ll << (8*3 + ((bslashId + 3)&7)) ) * !((7-row < bslashId)^(7-3 < bslashId))) | \
                        ((1ll << (8*4 + ((bslashId + 4)&7)) ) * !((7-row < bslashId)^(7-4 < bslashId))) | \
                        ((1ll << (8*5 + ((bslashId + 5)&7)) ) * !((7-row < bslashId)^(7-5 < bslashId))) | \
                        ((1ll << (8*6 + ((bslashId + 6)&7)) ) * !((7-row < bslashId)^(7-6 < bslashId))) | \
                        ((1ll << (8*7 + ((bslashId + 7)&7)) ) * !((7-row < bslashId)^(7-7 < bslashId)));

    return (\
    (~((1ll << (63 - __builtin_clzll( ((1ll<<tile)-1) & allBoard & slashMap ))) - 1)) & (slashMap & ((1ll<<tile)-1)) | \
    (~((1ll << (63 - __builtin_clzll( ((1ll<<tile)-1) & allBoard & bslashMap))) - 1)) & (bslashMap & ((1ll<<tile)-1)) | \
    (tile != 63)*((((allBoard & ~((1ll << (tile+1))-1) & bslashMap) & -(allBoard & ~((1ll << (tile+1))-1))) << 1) -1 \
    & (bslashMap & ~((1ll << (tile+1))-1))) | (tile != 63)*\
    ((((allBoard & ~((1ll << (tile+1))-1) & slashMap) & -(allBoard & ~((1ll << (tile+1))-1) & slashMap)) << 1) -1\
    & (slashMap & ~((1ll << (tile+1))-1))) ) & (~getColorBitBoard(color));
}


uint64_t CFBoard::getKnightPattern(int tile, bool color) {
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);

    return \
        ( (column > 0 && row > 1) * ((1ll << (tile - 17)) & ~allyBoard) ) | \
        ( (column < 7 && row > 1) * ((1ll << (tile - 15)) & ~allyBoard) ) | \
        ( (column > 1 && row > 0) * ((1ll << (tile - 10)) & ~allyBoard) ) | \
        ( (column < 6 && row > 0) * ((1ll << (tile -  6)) & ~allyBoard) ) | \
        ( (column > 1 && row < 7) * ((1ll << (tile +  6)) & ~allyBoard) ) | \
        ( (column < 6 && row < 7) * ((1ll << (tile + 10)) & ~allyBoard) ) | \
        ( (column > 0 && row < 6) * ((1ll << (tile + 15)) & ~allyBoard) ) | \
        ( (column < 7 && row < 6) * ((1ll << (tile + 17)) & ~allyBoard) );
}


uint64_t CFBoard::getKingPattern(int tile, bool color) {
    uint64_t kingPattern = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);

    kingPattern = \
    (1ll << (tile - 1))*(column > 0) | \
    (1ll << (tile - 8))*(row > 0) | \
    (1ll << (tile + 1))*(column < 7) | \
    (1ll << (tile + 8))*(row < 7) | \
    (1ll << (tile - 9))*(column > 0 && row > 0) | \
    (1ll << (tile + 7))*(column > 0 && row < 7) | \
    (1ll << (tile - 7))*(column < 7 && row > 0) | \
    (1ll << (tile + 9))*(column < 7 && row < 7);

    if (tile!=60 && tile!=4){
        return kingPattern;
    }

    //castle
    int castle;
    if (color){
        castle = castleCheck >> 2;
    } else {
        castle = castleCheck & 3;
    }

    // WARNING: this makes a handful of assumptions.
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


uint64_t CFBoard::getLegalMoves(int pieceId, int tile) {
    bool color = pieceId & 1;
    uint64_t retBoard;

	switch (pieceId >> 1) {
    case 0: // pawn
        retBoard = getPawnPattern(tile, color);
		break;
    case 1: // knight
        retBoard = getKnightPattern(tile, color);
		break;
    case 2: // bishop
        retBoard = getDiagonals(tile, color);
		break;
    case 3: // rook
        retBoard = getCardinals(tile, color);
		break;
    case 4: // queen
        retBoard = getDiagonals(tile, color) | getCardinals(tile, color);
		break;
    case 5: // king
        retBoard = getKingPattern(tile, color);
		break;
	default : //failsafe
		return 0;

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


void CFBoard::backupState() {
	//if this is our first backup, initialize the arrays
	if (backupStock == 0) {
		pawnBoardBackups = new uint64_t[backupCount];
		knightBoardBackups = new uint64_t[backupCount];
		bishopBoardBackups = new uint64_t[backupCount];
		rookBoardBackups = new uint64_t[backupCount];
		queenBoardBackups = new uint64_t[backupCount];
		kingBoardBackups = new uint64_t[backupCount];

		blackBoardBackups = new uint64_t[backupCount];
		whiteBoardBackups = new uint64_t[backupCount];

		enPassantTargetBackups = new int[backupCount];
		castleCheckBackups = new int[backupCount];
		isStateLegalBackups = new bool[backupCount];
	}
	else { //roll all backups forward
		for (int i = backupCount - 1; i >= 1; i--) {
			pawnBoardBackups[i] = pawnBoardBackups[i - 1];
			knightBoardBackups[i] = knightBoardBackups[i - 1];
			bishopBoardBackups[i] = bishopBoardBackups[i - 1];
			rookBoardBackups[i] = rookBoardBackups[i - 1];
			queenBoardBackups[i] = queenBoardBackups[i - 1];
			kingBoardBackups[i] = kingBoardBackups[i - 1];

			blackBoardBackups[i] = blackBoardBackups[i - 1];
			whiteBoardBackups[i] = whiteBoardBackups[i - 1];

			enPassantTargetBackups[i] = enPassantTargetBackups[i - 1];
			castleCheckBackups[i] = castleCheckBackups[i-1];
			isStateLegalBackups[i] = isStateLegalBackups[i-1];


		}
	}

	//now save the current state at the first index
	pawnBoardBackups[0] = pawnBoard;
	knightBoardBackups[0] = knightBoard;
	bishopBoardBackups[0] = bishopBoard;
	rookBoardBackups[0] = rookBoard;
	queenBoardBackups[0] = queenBoard;
	kingBoardBackups[0] = kingBoard;

	blackBoardBackups[0] = blackBoard;
	whiteBoardBackups[0] = whiteBoard;

	enPassantTargetBackups[0] = enPassantTarget;
	castleCheckBackups[0] = castleCheck;
	isStateLegalBackups[0] = isStateLegal;

	//increase the backup stock
	backupStock++;
}