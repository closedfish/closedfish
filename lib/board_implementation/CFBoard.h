#pragma once

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
    std::string getRepr();
    std::string getReprLegalMove(int pieceId, int tile);
  
    char pieceIdToChar(int pieceId);
    int pieceCharToId(char pieceChar);

    // ----- Get functions -----

    uint64_t getColorBitBoard(bool color);
    uint64_t getPieceColorBitBoard(int pieceId);
    uint64_t &getPieceBoardFromIndex(int boardIndex);
    
    bool getCurrentPlayer();
    int getPieceFromCoords(int tile);
    bool getBit(int pieceId, int tile);
    int getMaterialCount(bool color);

    std::string tileToCoords(int tile);
    std::string getNextMoveRepr(int startTile, int endTile);

    // ----- Manipulation -----

    void addPiece(int pieceId, int tile);
    void removePiece(int tile);

    void movePiece(int starttile, int endtile);
    void forceUndo(int startTileLastTurn, int endTileLastTurn, int capturedPiece);

    // ----- Ruleset -----
    uint64_t getCardinals(int tile, bool color);
    uint64_t getDiagonals(int tile, bool color);
    uint64_t getKnightPattern(int tile, bool color);
    uint64_t getKingPattern(int tile, bool color);
    uint64_t getPawnPattern(int tile, bool color);
    uint64_t getLegalMoves(int pieceId, int tile);

    bool naiveCheckCheck(bool color, int coordA = -1, int coordB = -1);

    // Misc

    friend bool operator==(const CFBoard &board1, const CFBoard &board2) {
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
    uint64_t pawnBoard;
    uint64_t knightBoard;
    uint64_t bishopBoard;
    uint64_t rookBoard;
    uint64_t queenBoard;
    uint64_t kingBoard;

    // TODO reduce both of these to the smallest int types
    int enPassantTarget; // a single coordinate from 0-63
    int castleCheck; // 4 bits of information
                     //(long black - short black- long white - short white)

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
