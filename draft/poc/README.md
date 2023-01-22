# POC -- Proof of Concepts

Any idea or working components we want to use but haven't yet finalized in our plan,
i.e. not sure if we're definitely using it, would be in this directory.
This includes (but not limited to): UI demos, pending (unfinished) bitboards,
unfinished C++ functions, etc.

## try_interface.cpp

Compilation command: `g++ -std=c++17 -O2 logger.cpp try_interface.cpp ../../lib/board_implementation/CFBoard.cpp ../../lib/board_implementation/naiveCheckCheck.cpp ../../external/stockfish/src/benchmark.o ../../external/stockfish/src/bitbase.o ../../external/stockfish/src/bitboard.o ../../external/stockfish/src/endgame.o ../../external/stockfish/src/evaluate.o ../../external/stockfish/src/material.o ../../external/stockfish/src/misc.o ../../external/stockfish/src/movegen.o ../../external/stockfish/src/movepick.o ../../external/stockfish/src/pawns.o ../../external/stockfish/src/position.o ../../external/stockfish/src/psqt.o ../../external/stockfish/src/search.o ../../external/stockfish/src/thread.o ../../external/stockfish/src/timeman.o ../../external/stockfish/src/tt.o ../../external/stockfish/src/uci.o ../../external/stockfish/src/ucioption.o ../../external/stockfish/src/tune.o  -o try_interface`

### Updated compilation command

Run:

`g++ -std=c++17 -O2 logger.cpp ../../lib/board_implementation/CFBoard.cpp ../../lib/board_implementation/naiveCheckCheck.cpp ../../external/stockfish/src/benchmark.cpp ../../external/stockfish/src/bitbase.cpp ../../external/stockfish/src/bitboard.cpp ../../external/stockfish/src/endgame.cpp ../../external/stockfish/src/evaluate.cpp ../../external/stockfish/src/material.cpp ../../external/stockfish/src/misc.cpp ../../external/stockfish/src/movegen.cpp ../../external/stockfish/src/movepick.cpp ../../external/stockfish/src/pawns.cpp ../../external/stockfish/src/position.cpp ../../external/stockfish/src/psqt.cpp ../../external/stockfish/src/search.cpp ../../external/stockfish/src/thread.cpp ../../external/stockfish/src/timeman.cpp ../../external/stockfish/src/tt.cpp ../../external/stockfish/src/uci.cpp ../../external/stockfish/src/ucioption.cpp ../../external/stockfish/src/tune.cpp ../../external/stockfish/src/syzygy/tbprobe.cpp ../../external/stockfish/src/nnue/evaluate_nnue.cpp ../../external/stockfish/src/nnue/features/half_ka_v2_hm.cpp -c -static`

to generate object files without linking first. Then,

`g++ -std=c++17 -O2 try_interface.cpp logger.o CFBoard.o naiveCheckCheck.o benchmark.o bitbase.o bitboard.o endgame.o evaluate.o material.o misc.o movegen.o movepick.o pawns.o position.o psqt.o search.o thread.o timeman.o tt.o uci.o ucioption.o tune.o tbprobe.o evaluate_nnue.o half_ka_v2_hm.o -static -o try_interface`

to compile. (This saves the compilation time because the files above are not changed)
