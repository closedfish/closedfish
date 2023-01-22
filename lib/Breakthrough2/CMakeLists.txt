set(BT2_SOURCES 
    "Breakthrough2.cpp" "stockfish_naive_eval.cpp")
set(BT2_HEADERS
    "Breakthrough2.h" "stockfish_naive_eval.h")

add_library(${BT2} STATIC
    ${BT2_SOURCES}
    ${BT2_HEADERS})
    
target_include_directories(${BT2} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${BT2} PUBLIC ${BI} ${WRAP} ${WEAKP} ${HMP} ${UTILS})

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${BT2} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${BT2} optimized)
endif()