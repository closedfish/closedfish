## Summary

`stockfish_naive_eval`: Evaluate our `CFBoard` using Stockfish's naive evaluation.

TODO: add optional parameter depth

**Note**: The initial tests are wrong, but in my opinion I think it's close (and good enough) to one provided here: https://hxim.github.io/Stockfish-Evaluation-Guide/?p=rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP1K2/PpP1N3/1P6/RNBQ1BR1%20w%20-%20-%200%201

## Compilation:

`g++ -std=c++17 -O2 main.cpp stockfish_naive_eval.cpp ../../lib/board_implementation/CFBoard.cpp ../../lib/board_implementation/naiveCheckCheck.cpp ../../external/stockfish/src/benchmark.cpp ../../external/stockfish/src/bitbase.cpp ../../external/stockfish/src/bitboard.cpp ../../external/stockfish/src/endgame.cpp ../../external/stockfish/src/evaluate.cpp ../../external/stockfish/src/material.cpp ../../external/stockfish/src/misc.cpp ../../external/stockfish/src/movegen.cpp ../../external/stockfish/src/movepick.cpp ../../external/stockfish/src/pawns.cpp ../../external/stockfish/src/position.cpp ../../external/stockfish/src/psqt.cpp ../../external/stockfish/src/search.cpp ../../external/stockfish/src/thread.cpp ../../external/stockfish/src/timeman.cpp ../../external/stockfish/src/tt.cpp ../../external/stockfish/src/uci.cpp ../../external/stockfish/src/ucioption.cpp ../../external/stockfish/src/tune.cpp ../../external/stockfish/src/syzygy/tbprobe.cpp ../../external/stockfish/src/nnue/evaluate_nnue.cpp ../../external/stockfish/src/nnue/features/half_ka_v2_hm.cpp -o main -DNNUE_EMBEDDING_OFF`
