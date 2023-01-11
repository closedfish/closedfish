#include "DFS1P.h"
#include <vector>
#include <queue>
#include <tuple>
#include <array>
#include <algorithm>
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
std::array<std::array<int, 8>, 8> distFromTileToTilesAsPiece(CFBoard& board, int halfPieceId, int startTile) {
    std::queue<std::tuple<int,int>> q;
    std::array<std::array<int, 8>, 8> dist;
    bool currentTurn = board.getCurrentPlayer(); // 0: white, 1: black
    uint64_t opponentPawnBoard = board.getPieceColorBitBoard(!currentTurn);
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) dist[i][j] = -1;
    dist[startTile/8][startTile%8] = 0;
    q.push(std::make_tuple(startTile/8, startTile%8));
    // BFS to find shortest distance from startTile to endTile
    while(!q.empty()) {
        int curi = std::get<0>(q.front()), curj = std::get<1>(q.front());
        q.pop();

        uint64_t nextSquares = board.getLegalMoves(2*halfPieceId + currentTurn, curi*8 + curj);
        // cout << nextSquares << '\n';
        for (int newTile: bitSetPositions(nextSquares)) {
            int newi = newTile/8;
            int newj = newTile%8;
            // cout << newi << ' ' << newj << ' ' << (board.getPieceFromCoords(newi*8 + newj)) << '\n';
            if (!Heatmap::validSquare(newi, newj)) continue; // out of bound
            if (dist[newi][newj] != -1) continue; // already visited
            if (board.getPieceFromCoords(newi*8+newj) != -1) continue; // our piece or opponent piece already on that square
            if (!squareSafeFromOpponentPawns(currentTurn, opponentPawnBoard, newi, newj)) continue; // square unsafe for our piece
            dist[newi][newj] = dist[curi][curj] + 1;
            q.push(std::make_tuple(newi, newj));
        }
    }
    return dist;
}

int distFromHeatmap(CFBoard& board, int (&heatMap)[6][8][8]) {
    int dist = 0;
    const int COEFF_SEPARATED = 100; // adjustable
    bool currentTurn = board.getCurrentPlayer(); // 0: white, 1: black
    for (int halfPieceId = 0; halfPieceId < 6; halfPieceId++) {
        // if (halfPieceId != 1) continue;
        // cout << "Dist map for: " << halfPieceId << '\n';
        uint64_t pieceBoard = board.getPieceColorBitBoard(2*halfPieceId|currentTurn);
        std::vector<int> pieceTiles = bitSetPositions(pieceBoard);
        for (int startTile: pieceTiles) {
            std::array<std::array<int, 8>, 8> distFromStart = distFromTileToTilesAsPiece(board, halfPieceId, startTile);
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    // cout << heatMap[halfPieceId][i][j] << ' ';
                    // cout << distFromStart[i][j] << ' ';
                    if (heatMap[halfPieceId][i][j] == 0) continue;
                    if (distFromStart[i][j] != -1) {
                        dist += heatMap[halfPieceId][i][j] * distFromStart[i][j];
                    } else {
                        dist += COEFF_SEPARATED*heatMap[halfPieceId][i][j];
                    }
                }
                // cout << '\n';
            }
        }
    }
    // std::cout << dist << '\n';
    return dist;
}

void DFS1pAux(CFBoard* currentBoard, int depth, int maxDepth, std::vector<std::tuple<int, int, float>> curLine, std::vector<std::vector<std::tuple<int, int, float>>>& possibleLines) {
    if (depth == maxDepth) {
        possibleLines.push_back(curLine);
        return;
    }

    bool currentTurn = currentBoard->getCurrentPlayer(); // 0: white, 1: black
    for (int startTile = 0; startTile < 64; startTile++) {
        int pieceId = currentBoard->getPieceFromCoords(startTile);
        if (pieceId == -1 || pieceId&1 != currentTurn) continue;
        // cout << "piece: " << pieceId << '\n';
        uint64_t legalMoves =  currentBoard->getLegalMoves(pieceId, startTile);
        std::vector<int> endTiles = bitSetPositions(legalMoves);
        for (int endTile: endTiles) {
            // std::cout << startTile << ' ' << endTile << '\n';
            if (currentBoard->getPieceFromCoords(endTile) != -1) continue; // opponent pieces already on that square
            if (!squareSafeFromOpponentPawns(currentTurn,
            currentBoard->getPieceColorBitBoard(!currentTurn), endTile/8, endTile%8)) continue; // square unsafe for our piece
            curLine.push_back(std::make_tuple(startTile, endTile, 0.0));
            // cout << "current turn: " << currentTurn << '\n';
            currentBoard->movePiece(startTile, endTile);
            currentBoard->forceFlipTurn();
            // cout << "recent move: " << startTile << ' ' << endTile << '\n';
            // cout << currentBoard->getRepr();
            DFS1pAux(currentBoard, depth+1, maxDepth, curLine, possibleLines);
            curLine.pop_back();
            currentBoard->undoLastMove();
            currentBoard->forceFlipTurn();
        }
    }
}

std::tuple<int, int, float> DFS1P::getNextMove() {
    int heatMap[6][8][8];
    memset(heatMap, 0, sizeof heatMap);
	uint64_t weakPawns = 8ll; // placeholder for finished weak pawns implementation
    Heatmap::addHeatMap(*currentBoard, heatMap, weakPawns);

    int maxDepth = 3; // CUSTOMIZABLE MAX DEPTH FOR DFS, CAPPED AT 4 CURRENTLY
    // Get all possible moves
    std::vector<std::tuple<int, int, float>> ansLine;
    std::vector<std::vector<std::tuple<int, int, float>>> possibleLines;
    
    // Check all moves (currently only look 1 turn ahead)
    int minDist = 1e9;
    DFS1pAux(currentBoard, 0, maxDepth, {}, possibleLines);

    for (auto line: possibleLines) {
        // if (std::get<0>(line[0]) != 54 || std::get<1>(line[0])  != 46) continue;
        // cout << "line: \n";
        // cout << "dist before: " << distFromHeatmap(*currentBoard, heatMap) << '\n';
        // cout << currentBoard->getRepr();
        // cout << "in check: " << currentBoard->naiveCheckCheck(currentBoard->getCurrentPlayer()) << '\n';
        for (auto move: line) {
            // cout << "move: " << std::get<0>(move) << ' ' << std::get<1>(move) << '\n';
            // cout << "current turn: " << currentBoard->getCurrentPlayer() << '\n';
            // cout << "next move: " << std::get<0>(move) << ' ' << std::get<1>(move) << '\n';
            // cout << "current board: \n" << currentBoard->getRepr() << '\n'  << " current distance: " << distFromHeatmap(*currentBoard, heatMap) << '\n';
            currentBoard->movePiece(std::get<0>(move), std::get<1>(move));
            currentBoard->forceFlipTurn();
            // cout << "turn after moving: " << currentBoard->getCurrentPlayer() << '\n';
            // cout << "board after moving: \n" << currentBoard->getRepr() << '\n' << " distance after moving: " << distFromHeatmap(*currentBoard, heatMap) << "\n \n";
            // Check if the move make us closer to the heatMap
            
        }
        if (currentBoard->naiveCheckCheck(currentBoard->getCurrentPlayer())) {
            cout << "REDFLAG: SHOULDN'T BE IN CHECK\n";
            cout << "dist after: " << distFromHeatmap(*currentBoard, heatMap) << '\n';
            cout << currentBoard->getRepr();
            cout << currentBoard->toFEN();
            cout << "in check: " << currentBoard->naiveCheckCheck(currentBoard->getCurrentPlayer()) << '\n';
        }
        if (distFromHeatmap(*currentBoard, heatMap) < minDist) {
            minDist = distFromHeatmap(*currentBoard, heatMap);
            // If yes then update the potential move
            ansLine = line;
        }
        for (auto move: line) {
            currentBoard->undoLastMove();
            currentBoard->forceFlipTurn();
        }
    }
    // for (auto move: ansLine) {
    //     cout << std::get<0>(move) << ' ' << std::get<1>(move) << '\n';
    // }
    cout << distFromHeatmap(*currentBoard, heatMap) << '\n';
    return ansLine[0];

    // // Depth 1 only
    // // Get all possible moves
    // std::tuple<int, int, float> ansMove;
    // std::vector<std::tuple<int, int, float>> possibleMoves;
    // bool currentTurn = currentBoard->getCurrentPlayer(); // 0: white, 1: black
    // for (int startTile = 0; startTile < 64; startTile++) {
    //     int pieceId = currentBoard->getPieceFromCoords(startTile);
    //     if (pieceId&1 != currentTurn) continue;
    //     uint64_t legalMoves =  currentBoard->getLegalMoves(pieceId, startTile);
    //     std::vector<int> endTiles = bitSetPositions(legalMoves);
    //     for (int endTile: endTiles) {
    //         // std::cout << startTile << ' ' << endTile << '\n';
    //         possibleMoves.push_back(std::make_tuple(startTile, endTile, 0.0));
    //     }
    // }
    
    // // Check all moves (currently only look 1 turn ahead)
    // int minDist = 1e9;
    // for (auto move: possibleMoves) {
    //     // cout << "current turn: " << currentBoard->getCurrentPlayer() << '\n';
    //     // cout << "next move: " << std::get<0>(move) << ' ' << std::get<1>(move) << '\n';
    //     // cout << "current board: \n" << currentBoard->getRepr() << '\n'  << " current distance: " << distFromHeatmap(*currentBoard, heatMap) << '\n';
    //     currentBoard->movePiece(std::get<0>(move), std::get<1>(move));
    //     // cout << "turn after moving: " << currentBoard->getCurrentPlayer() << '\n';
    //     // cout << "board after moving: \n" << currentBoard->getRepr() << '\n' << " distance after moving: " << distFromHeatmap(*currentBoard, heatMap) << "\n \n";
    //     // Check if the move make us closer to the heatMap
    //     if (distFromHeatmap(*currentBoard, heatMap) < minDist) {
    //         minDist = distFromHeatmap(*currentBoard, heatMap);
    //         // If yes then update the potential move
    //         ansMove = move;
    //     }
    //     currentBoard->undoLastMove();
        
    // }

    // return ansMove;
}

int main() {
    DFS1P algo;
    CFBoard board = CFBoard("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP5/1P4K1/RNBQ1B1R w - - 0 1");
    // CFBoard board = CFBoard("rkqrbnnb/8/p5p1/Pp1p1pPp/1PpPpP1P/2P1P1N1/2B1QB1R/3K3R w - - 0 1"); // no open files, >= 2 free rows
	// CFBoard board = CFBoard("rkqr1nnb/4b3/8/p3p1p1/Pp1pPpPp/1PpP1P1P/R1P4N/1NKQBB1R w - - 0 1"); // no open files, 1 free rows, no chance of winning, 3 is better than 4 for some reasons
    // CFBoard board = CFBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");
    // CFBoard board = CFBoard("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP4/PpP1BN1R/1P2QNK1/4RB2 w - - 0"); // wrong old naiveCheckCheck
    // CFBoard board = CFBoard("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP1B2/PpP3QR/1P1N1N2/R4BK1 w - - 0 1"); // wrong legal moves for bishop
    // cout << board.naiveCheckCheck(0) << '\n';
    // cout << board.getReprLegalMove(6, 47); 

    algo.setBoardPointer(&board);
    // cout << board.getRepr() << '\n';

    // // Test dist between tiles
    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         int tile = i*8 + j;
    //         cout << distFromTileToTileAsPiece(board, 2, 61, tile) << ' ';
    //     }
    //     cout << '\n';
    // }

    // for (int i = 0; i < 15; i++) {
    //     auto move = algo.getNextMove();
    //     int startTile = std::get<0>(move), endTile = std::get<1>(move);
    //     float eval = std::get<2>(move);
    //     std::cout << startTile << ' ' << endTile << ' ' << eval << '\n';
    //     board.movePiece(startTile, endTile);
    //     board.forceFlipTurn(); // One person moving only
    //     cout << board.getRepr() << '\n';
    // }
}