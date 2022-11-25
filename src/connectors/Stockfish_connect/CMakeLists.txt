set(SC_SOURCES 
    "Stockfish_connect_main.cc")
set(SC_HEADERS
    "Stockfish_connect_main.hpp")

add_library(${SC} STATIC
    ${SC_SOURCES}
    ${SC_HEADERS})
    
target_include_directories(${SC} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${SC} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${SC} optimized)
endif()