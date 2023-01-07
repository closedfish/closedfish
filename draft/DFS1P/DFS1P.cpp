#include "DFS1P.h"
#include <vector>
#include <queue>
#include <tuple>
using std::cout;

void DFS1P::setBoardPointer(CFBoard* ptr) {
    if (ptr)
        currentBoard = ptr;
}

std::vector<int> di[6], dj[6];
void initdidj() {
    di[1] = {1, 2, 2, 1, -1, -2, -2, -1};
    dj[1] = {2, 1, -1, -2, -2, -1, 1, 2};

    for (int k = -7; k < 7; k++) {
        if (k == 0) continue;
        di[2].push_back(k);
        dj[2].push_back(k);
        di[2].push_back(k);
        dj[2].push_back(-k);

        di[3].push_back(k);
        dj[3].push_back(0);
        di[3].push_back(0);
        dj[3].push_back(k);

        di[4].push_back(k);
        dj[4].push_back(k);
        di[4].push_back(k);
        dj[4].push_back(-k);
        di[4].push_back(k);
        dj[4].push_back(0);
        di[4].push_back(0);
        dj[4].push_back(k);
    }
    di[5] = {-1, -1, -1, 0, 0, 1, 1, 1};
    dj[5] = {-1, 0, 1, -1, 1, -1, 0, 1};
}

//Distance between two squares with respect to a piece's movement
int distFromTileToTileAsPiece(CFBoard& board, int halfPieceId, int startTile, int endTile) {
    std::queue<std::tuple<int,int>> q;
    int dist[8][8];
    bool currentTurn = board.getCurrentPlayer(); // 0: white, 1: black
    memset(dist, -1, sizeof dist);
    dist[startTile/8][startTile%8] = 0;
    q.push(std::make_tuple(startTile/8, startTile%8));
    // BFS to find shortest distance from startTile to endTile
    while(!q.empty()) {
        int curi = std::get<0>(q.front()), curj = std::get<1>(q.front());
        q.pop();
        if (curi*8 + curj == endTile) return dist[curi][curj];
        for (int k = 0; k < di[halfPieceId].size(); k++) {
            int newi = curi+di[halfPieceId][k];
            int newj = curj+dj[halfPieceId][k];
            if (!Heatmap::validSquare(newi, newj)) continue; // out of bound
            if (dist[newi][newj] != -1) continue; // already visited
            if (board.getPieceFromCoords(newi*8 + newj)&1 == currentTurn) continue; // our piece already on that square
            dist[newi][newj] = dist[curi][curj] + 1;
        }
    }
    return -1;
}

int distFromHeatmap(CFBoard& board, int (&heatMap)[6][8][8]) {
    int dist = 0;
    bool currentTurn = board.getCurrentPlayer(); // 0: white, 1: black
    for (int halfPieceId = 0; halfPieceId < 6; halfPieceId++) {
        uint64_t pieceBoard = board.getPieceColorBitBoard(2*halfPieceId|currentTurn);
        std::vector<int> pieceTiles = bitSetPositions(pieceBoard);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (heatMap[halfPieceId][i][j] != 0) {
                    for (int startTile: pieceTiles)
                        dist += distFromTileToTileAsPiece(board, halfPieceId, startTile, i*8+j);
                }
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
    initdidj();
    // for (int halfPieceId = 0; halfPieceId < 6; halfPieceId++) {
    //     cout << halfPieceId << '\n';
    //     for (int k = 0; k < di[halfPieceId].size(); k++) {
    //         cout << di[halfPieceId][k] << ' ' << dj[halfPieceId][k] << '\n';
    //     }
    // }
    auto move = algo.getNextMove();
    std::cout << std::get<0>(move) << ' ' << std::get<1>(move) << ' ' << std::get<2>(move) << '\n';
}