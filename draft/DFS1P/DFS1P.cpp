#include "DFS1P.h"
#include <vector>
#include <tuple>
using std::cout;

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
                dist += heatMap[halfPieceId][i][j] - (pieceBoard & 1ll);
                pieceBoard >>= 1;
            }
        }
    }
    // std::cout << dist << '\n';
    return dist;
}

std::tuple<int, int, float> DFS1P::getNextMove() {
    int heatMap[6][8][8];
    memset(heatMap, 0, sizeof heatMap);
	uint64_t weakPawns = 1ll; // placeholder for finished weak pawns implementation
    Heatmap::addHeatMap(*currentBoard, heatMap, weakPawns);
    std::tuple<int, int, float> ansMove;


    // Get all possible moves
    std::vector<std::tuple<int, int, float>> possibleMoves;
    bool currentTurn = currentBoard->getCurrentPlayer(); // 0: white, 1: black
    for (int startTile = 0; startTile < 64; startTile++) {
        int pieceId = currentBoard->getPieceFromCoords(startTile);
        if (pieceId&1 != currentTurn) continue;
        uint64_t legalMoves =  currentBoard->getLegalMoves(pieceId, startTile);
        std::vector<int> endTiles = bitSetPositions(legalMoves);
        for (int endTile: endTiles) {
            // std::cout << startTile << ' ' << endTile << '\n';
            possibleMoves.push_back(std::make_tuple(startTile, endTile, 0.0));
        }
    }
    
    // Check all moves (currently only look 1 turn ahead)
    int minDist = 1e9;
    for (auto move: possibleMoves) {
        currentBoard->movePiece(std::get<0>(move), std::get<1>(move), -1);
        // Check if the move make us closer to the heatMap
        // cout << currentBoard->getRepr() << '\n' << distFromHeatmap(*currentBoard, heatMap) << '\n';
        if (distFromHeatmap(*currentBoard, heatMap) < minDist) {
            minDist = distFromHeatmap(*currentBoard, heatMap);
            // If yes then update the potential move
            ansMove = move;
        }
        currentBoard->undoLastMove();
        
    }

    return ansMove;
}

int main() {
    DFS1P algo;
    CFBoard board = CFBoard("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP5/1P4K1/RNBQ1B1R w - - 0 1");

    algo.setBoardPointer(&board);
    auto move = algo.getNextMove();
    std::cout << std::get<0>(move) << ' ' << std::get<1>(move) << ' ' << std::get<2>(move) << '\n';
}