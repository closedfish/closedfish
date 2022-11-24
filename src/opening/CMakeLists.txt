set(OPENING_SOURCES 
    "opening_main.cc")
set(OPENING_HEADERS
    "opening_main.hpp")

add_library(${OPENING} STATIC
    ${OPENING_SOURCES}
    ${OPENING_HEADERS})
    
target_include_directories(${OPENING} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")