# Including Stockfish

set(SF_SOURCES
		"stockfish/src/benchmark.cpp" "stockfish/src/bitbase.cpp" "stockfish/src/bitboard.cpp" "stockfish/src/endgame.cpp" "stockfish/src/evaluate.cpp"
		"stockfish/src/main.cpp" "stockfish/src/material.cpp" "stockfish/src/misc.cpp" "stockfish/src/movegen.cpp" "stockfish/src/movepick.cpp" "stockfish/src/pawns.cpp"
		"stockfish/src/position.cpp" "stockfish/src/psqt.cpp" "stockfish/src/search.cpp" "stockfish/src/thread.cpp" "stockfish/src/timeman.cpp" "stockfish/src/tt.cpp"
		"stockfish/src/uci.cpp" "stockfish/src/ucioption.cpp" "stockfish/src/tune.cpp" "stockfish/src/syzygy/tbprobe.cpp" "stockfish/src/nnue/evaluate_nnue.cpp"
		"stockfish/src/nnue/features/half_ka_v2_hm.cpp")
set(SF_HEADERS
    "stockfish/src/bitboard.h" "stockfish/src/endgame.h" "stockfish/src/evaluate.h" "stockfish/src/material.h" "stockfish/src/misc.h" "stockfish/src/movegen.h"
		"stockfish/src/pawns.h" "stockfish/src/position.h" "stockfish/src/psqt.h" "stockfish/src/search.h" "stockfish/src/thread_win32_osx.h" "stockfish/src/thread.h"
		"stockfish/src/timeman.h" "stockfish/src/tt.h" "stockfish/src/tune.h" "stockfish/src/types.h" "stockfish/src/uci.h" "stockfish/src/nnue/evaluate_nnue.h"
		"stockfish/src/nnue/nnue_accumulator.h" "stockfish/src/nnue/nnue_architecture.h" "stockfish/src/nnue/nnue_common.h" "stockfish/src/nnue/nnue_feature_transformer.h"
		"stockfish/src/nnue/features/half_ka_v2_hm.h" "stockfish/src/nnue/layers/affine_transform.h" "stockfish/src/nnue/layers/clipped_relu.h" "stockfish/src/nnue/layers/simd.h"
		"stockfish/src/nnue/layers/sqr_clipped_relu.h" "stockfish/src/syzygy/tbprobe.h")

add_library(${SF} STATIC
    ${SF_SOURCES}
    ${SF_HEADERS})
    
target_include_directories(${SF} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${SF} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${SF} optimized)
endif()