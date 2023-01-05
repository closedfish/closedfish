#include "DFS1P.h"
#include <vector>
#include <tuple>

void DFS1P::setBoardPointer(CFBoard* ptr) {
    if (ptr)
        currentBoard = ptr;
}

int distFromHeatmap(CFBoard& board, int (&heatMap)[6][8][8]) {
    int dist = 0;
    bool currentTurn = board.getCurrentPlayer(); // 0: white, 1: black
    for (int halfPieceId = 0; halfPieceId < 6; halfPieceId++) {
        uint64_t pieceBoard = board.getPieceColorBitBoard(2*halfPieceId|currentTurn);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                dist += heatMap[halfPieceId][i][j] - (pieceBoard & 1);
                pieceBoard >>= 1;
            }
        }
    }
    return dist;
}

std::tuple<int, int, float> DFS1P::getNextMove() {
    int heatMap[6][8][8];
	uint64_t weakPawns = 1ll; // placeholder for finished weak pawns implementation
    Heatmap::addHeatMap(*currentBoard, heatMap, weakPawns);
    int minDist = 1e9;
    std::tuple<int, int, float> ansMove;

    // Get all possible moves
    std::vector<std::tuple<int, int, float>> possibleMoves;
    bool currentTurn = currentBoard->getCurrentPlayer(); // 0: white, 1: black
    for (int halfPieceId = 0; halfPieceId < 6; halfPieceId++) {
        for (int startTile = 0; startTile < 64; startTile++) {
            uint64_t legalMoves =  currentBoard->getLegalMoves(2*halfPieceId+currentTurn, startTile);
            std::vector<int> endTiles = bitSetPositions(legalMoves);
            for (int endTile: endTiles) {
                possibleMoves.push_back(std::make_tuple(startTile, endTile, 0.0));
            }
        }
    }

    // Check all moves
    for (auto move: possibleMoves) {
        currentBoard->movePiece(std::get<0>(move), std::get<1>(move));
        // Check if the move make us closer to the heatMap
        if (distFromHeatmap(*currentBoard, heatMap) < minDist) {
            minDist = distFromHeatmap(*currentBoard, heatMap);
            // If yes then update the potential move
            ansMove = move;
        }
        currentBoard->undoLastMove();
    }

    return ansMove;
}

// int main() {
//     DFS1P algo;
//     CFBoard board;
//     board.fromFEN("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP5/1P4K1/RNBQ1B1R w - - 0 1");
//     std::cout << board.getRepr();

//     // algo.setBoardPointer(&board);
//     // std::cout << std::get<0>(algo.getNextMove()) << ' ' << std::get<1>(algo.getNextMove()) << ' ' << std::get<2>(algo.getNextMove()) << '\n';
// }