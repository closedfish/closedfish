# POC -- Proof of Concepts

Any idea or working components we want to use but haven't yet finalized in our plan,
i.e. not sure if we're definitely using it, would be in this directory.
This includes (but not limited to): UI demos, pending (unfinished) bitboards,
unfinished C++ functions, etc.

## try_interface.cpp

Compilation command: `g++-11 -std=c++17 -O2 logger.cpp try_interface.cpp ../../lib/board_implementation/CFBoard.cpp ../../lib/board_implementation/naiveCheckCheck.cpp ../../external/stockfish/src/benchmark.cpp ../../external/stockfish/src/bitbase.cpp ../../external/stockfish/src/bitboard.cpp ../../external/stockfish/src/endgame.cpp ../../external/stockfish/src/evaluate.cpp ../../external/stockfish/src/material.cpp ../../external/stockfish/src/misc.cpp ../../external/stockfish/src/movegen.cpp ../../external/stockfish/src/movepick.cpp ../../external/stockfish/src/pawns.cpp ../../external/stockfish/src/position.cpp ../../external/stockfish/src/psqt.cpp ../../external/stockfish/src/search.cpp ../../external/stockfish/src/thread.cpp ../../external/stockfish/src/timeman.cpp ../../external/stockfish/src/tt.cpp ../../external/stockfish/src/uci.cpp ../../external/stockfish/src/ucioption.cpp ../../external/stockfish/src/tune.cpp ../../external/stockfish/src/syzygy/tbprobe.cpp ../../external/stockfish/src/nnue/evaluate_nnue.cpp ../../external/stockfish/src/nnue/features/half_ka_v2_hm.cpp -o try_interface -DNNUE_EMBEDDING_OFF`
