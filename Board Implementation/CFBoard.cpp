#include <iostream>
#include <stdint.h>

//test

class CFBoard {
public:

    CFBoard();

    void addPiece(char pieceType, bool color, int tile); // pieceType in P,R,N,B,Q,K and 0<=tile<=63. Function replaces existing piece on the tile.
    void removePiece(int tile);
    //void naiveMovePiece(int starttile, int endtile);

    bool getBit(char pieceType, bool color, int tile);

    // these create a copy (I think)
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
    uint64_t one = 1;

    uint64_t PawnBoard;
    uint64_t KingBoard;
    uint64_t QueenBoard;
    uint64_t RookBoard;
    uint64_t KnightBoard;
    uint64_t BishopBoard;

    uint64_t BlackBoard;
    uint64_t WhiteBoard;

    bool turn; // 0 for white, 1 for black

    /*
        a8 = 2^0
        b8 = 2^1
        ...
        h1 = 2^63
    */
};

CFBoard::CFBoard() { //This is just the starter board.
    PawnBoard = (((one << 8) - 1) << 48) + (((one << 8) - 1) << 8);
    KingBoard = (one << 4) + (one << 60);
    QueenBoard = (one << 3) + (one << 59);
    RookBoard = (one << 0) + (one << 7) + (one << 56) + (one << 63);
    KnightBoard = (one << 1) + (one << 6) + (one << 57) + (one << 62);
    BishopBoard = (one << 2) + (one << 5) + (one << 58) + (one << 61);
    BlackBoard = (one << 16) - 1;
    WhiteBoard = ((one << 16) - 1) << 48;

    turn = 1;
}

void CFBoard::fromFEN(std::string FEN){
    // configure the current board from input FEN
}
std::string toFEN(){
    // convert the current board to FEN
    return "NotYetImplemented";
}

void CFBoard::addPiece(char pieceType, bool color, int tile) {
    uint64_t PieceBoard = one << tile;
    removePiece(tile);

    switch (pieceType) {
    case 'P':
        PawnBoard = PawnBoard | PieceBoard;
        break;
    case 'K':
        KingBoard = KingBoard | PieceBoard;
        break;
    case 'Q':
        QueenBoard = QueenBoard | PieceBoard;
        break;
    case 'R':
        RookBoard = RookBoard | PieceBoard;
        break;
    case 'N':
        KnightBoard = KnightBoard | PieceBoard;
        break;
    case 'B':
        BishopBoard = BishopBoard | PieceBoard;
        break;
    }

    if (color) {
        BlackBoard = BlackBoard | PieceBoard;
    }
    else {
        WhiteBoard = WhiteBoard | PieceBoard;
    }
}
void CFBoard::removePiece(int tile) {
    uint64_t AntiPieceBoard = ~(one << tile);
    PawnBoard = PawnBoard & AntiPieceBoard;
    KingBoard = KingBoard & AntiPieceBoard;
    QueenBoard = QueenBoard & AntiPieceBoard;
    RookBoard = RookBoard & AntiPieceBoard;
    KnightBoard = KnightBoard & AntiPieceBoard;
    BishopBoard = BishopBoard & AntiPieceBoard;
    BlackBoard = BlackBoard & AntiPieceBoard;
    WhiteBoard = WhiteBoard & AntiPieceBoard;
    BlackBoard = BlackBoard & AntiPieceBoard;
    WhiteBoard = WhiteBoard & AntiPieceBoard;
}

//void CFBoard::naiveMovePiece(int starttile, int endtile) {}

bool CFBoard::getBit(char pieceType, bool color, int tile) {
    uint64_t Board = getPieceBitBoard(pieceType) & getColorBitBoard(color);
    return (Board << tile);
}

uint64_t CFBoard::getPieceBitBoard(char pieceType) {
    switch (pieceType) {
    case 'P':
        return PawnBoard;
    case 'K':
        return KingBoard;
    case 'Q':
        return QueenBoard;
    case 'R':
        return RookBoard;
    case 'N':
        return KnightBoard;
    case 'B':
        return BishopBoard;
    }
}
uint64_t CFBoard::getColorBitBoard(bool color) {
    if (color) {
        return BlackBoard;
    }
    return WhiteBoard;
}

std::string CFBoard::getRepr() {
    std::string repr = "|";
    for (int i = 0; i < 64; i++) {
        repr += " ";
        if ((BlackBoard >> i) & 1) {

            if ((PawnBoard >> i) & 1) {
                repr += "p";
            }
            else if ((KingBoard >> i) & 1) {
                repr += "k";
            }
            else if ((QueenBoard >> i) & 1) {
                repr += "q";
            }
            else if ((RookBoard >> i) & 1) {
                repr += "r";
            }
            else if ((KnightBoard >> i) & 1) {
                repr += "n";
            } else {
                repr += "b";
            }
        }
        else if ((WhiteBoard >> i) & 1) {
            if ((PawnBoard >> i) & 1) {
                repr += "P";
            }
            else if ((KingBoard >> i) & 1) {
                repr += "K";
            }
            else if ((QueenBoard >> i) & 1) {
                repr += "Q";
            }
            else if ((RookBoard >> i) & 1) {
                repr += "R";
            }
            else if ((KnightBoard >> i) & 1) {
                repr += "N";
            } else {
                repr += "B";
            }
        }
        else {
            repr += ".";
        }
        repr += " |";
        if ((i + 1) % 8 == 0) {
            repr += "\n|";
        }
    }
    return repr;
}