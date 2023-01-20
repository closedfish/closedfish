set(WEAKP_SOURCES 
    "PieceMovements.cpp" "WeakPawns.cpp")
set(WEAKP_HEADERS
    "PieceMovements.h" "WeakPawns.h")

add_library(${WEAKP} STATIC
    ${WEAKP_SOURCES}
    ${WEAKP_HEADERS})
    
target_include_directories(${WEAKP} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${WEAKP} PUBLIC ${BI})

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${WEAKP} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${WEAKP} optimized)
endif()