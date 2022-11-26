set(PLAY_SOURCES 
    "play_main.cc")
set(PLAY_HEADERS
    "play_main.hpp")

add_library(${PLAY} STATIC
    ${PLAY_SOURCES}
    ${PLAY_HEADERS})
    
target_include_directories(${PLAY} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")