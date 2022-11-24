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