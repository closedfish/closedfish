set(INPUT_SOURCES 
    "input.cc")
set(INPUT_HEADERS
    "input.hpp")

add_library(${INPUT} STATIC
    ${INPUT_SOURCES}
    ${INPUT_HEADERS})
    
target_include_directories(${INPUT} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")