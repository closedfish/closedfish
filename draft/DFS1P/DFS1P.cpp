#include "DFS1P.h"
#include <vector>
#include <queue>
#include <tuple>
using std::cout;

void DFS1P::setBoardPointer(CFBoard* ptr) {
    if (ptr)
        currentBoard = ptr;
}

bool squareSafeFromOpponentPawns(const bool &currentTurn, const uint64_t& opponentPawnBoard, const int& row, const int &col) {
    if (currentTurn)
	    return row == 7 || ((col == 0 || !isBitSet(opponentPawnBoard, Heatmap::posToTile({row+1, col-1})))
				&& (col == 7 || !isBitSet(opponentPawnBoard, Heatmap::posToTile({row+1, col+1}))));
    else
        return row == 0 || ((col == 0 || !isBitSet(opponentPawnBoard, Heatmap::posToTile({row-1, col-1})))
				&& (col == 7 || !isBitSet(opponentPawnBoard, Heatmap::posToTile({row-1, col+1}))));
}

//Distance between two squares with respect to a piece's movement
int distFromTileToTileAsPiece(CFBoard& board, int halfPieceId, int startTile, int endTile) {
    std::queue<std::tuple<int,int>> q;
    int dist[8][8];
    bool currentTurn = board.getCurrentPlayer(); // 0: white, 1: black
    uint64_t opponentPawnBoard = board.getPieceColorBitBoard(!currentTurn);
    memset(dist, -1, sizeof dist);
    dist[startTile/8][startTile%8] = 0;
    q.push(std::make_tuple(startTile/8, startTile%8));
    // BFS to find shortest distance from startTile to endTile
    while(!q.empty()) {
        int curi = std::get<0>(q.front()), curj = std::get<1>(q.front());
        q.pop();
        if (curi*8 + curj == endTile) return dist[curi][curj];

        uint64_t nextSquares = board.getLegalMoves(2*halfPieceId + currentTurn, curi*8 + curj);
        for (int newTile: bitSetPositions(nextSquares)) {
            int newi = newTile/8;
            int newj = newTile%8;
            if (!Heatmap::validSquare(newi, newj)) continue; // out of bound
            if (dist[newi][newj] != -1) continue; // already visited
            if (board.getPieceFromCoords(newi*8+newj) != -1) continue; // our piece or opponent piece already on that square
            if (!squareSafeFromOpponentPawns(currentTurn, opponentPawnBoard, newi, newj)) continue;
            // cout << newi << ' ' << newj << ' ' << (board.getPieceFromCoords(newi*8 + newj)) << '\n';
            dist[newi][newj] = dist[curi][curj] + 1;
            q.push(std::make_tuple(newi, newj));
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
                    for (int startTile: pieceTiles) {
                        if (distFromTileToTileAsPiece(board, halfPieceId, startTile, i*8+j) == -1) continue;
                        dist += distFromTileToTileAsPiece(board, halfPieceId, startTile, i*8+j);
                    }
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
        // cout << "current turn: " << currentBoard->getCurrentPlayer() << '\n';
        // cout << "next move: " << std::get<0>(move) << ' ' << std::get<1>(move) << '\n';
        // cout << "current board: \n" << currentBoard->getRepr() << '\n'  << " current distance: " << distFromHeatmap(*currentBoard, heatMap) << '\n';
        currentBoard->movePiece(std::get<0>(move), std::get<1>(move));
        // cout << "turn after moving: " << currentBoard->getCurrentPlayer() << '\n';
        // cout << "board after moving: \n" << currentBoard->getRepr() << '\n' << " distance after moving: " << distFromHeatmap(*currentBoard, heatMap) << "\n \n";
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

int main() {
    DFS1P algo;
    CFBoard board = CFBoard("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP5/1P4K1/RNBQ1B1R w - - 0 1");
    // CFBoard board = CFBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");

    algo.setBoardPointer(&board);
    cout << board.getRepr() << '\n';

    // // Test di, dj
    // for (int halfPieceId = 0; halfPieceId < 6; halfPieceId++) {
    //     cout << halfPieceId << '\n';
    //     for (int k = 0; k < di[halfPieceId].size(); k++) {
    //         cout << di[halfPieceId][k] << ' ' << dj[halfPieceId][k] << '\n';
    //     }
    // }

    // // Test dist between tiles
    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         int tile = i*8 + j;
    //         cout << distFromTileToTileAsPiece(board, 2, 61, tile) << ' ';
    //     }
    //     cout << '\n';
    // }

    for (int i = 0; i < 10; i++) {
        auto move = algo.getNextMove();
        int startTile = std::get<0>(move), endTile = std::get<1>(move);
        float eval = std::get<2>(move);
        std::cout << startTile << ' ' << endTile << ' ' << eval << '\n';
        board.movePiece(startTile, endTile);
        board.forceMovePiece(0, 0); // One person moving only
        cout << board.getRepr() << '\n';
    }
}