#pragma once

#include "WeakPawns.h"

uint64_t getDangerousTiles(CFBoard board, bool color);
bool isTileDangerous(CFBoard board, bool color, int tile);
int canPieceMoveOut(CFBoard &board, int tile);
uint64_t *getPieceMovements(CFBoard &board, bool &color, int tile);
int getLastMove(CFBoard lastBoard, CFBoard currentBoard, bool colorPlayed);