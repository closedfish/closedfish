#include "../../external/stockfish/src/bitboard.h"
#include "../../external/stockfish/src/endgame.h"
#include "../../external/stockfish/src/evaluate.h"
#include "../../external/stockfish/src/position.h"
#include "../../external/stockfish/src/psqt.h"
#include "../../external/stockfish/src/search.h"
#include "../../external/stockfish/src/syzygy/tbprobe.h"
#include "../../external/stockfish/src/thread.h"
#include "../../external/stockfish/src/tt.h"
#include "../../external/stockfish/src/uci.h"
#include "../../lib/board_implementation/CFBoard.h"

Stockfish::Value stockfishNaiveEval(CFBoard board);