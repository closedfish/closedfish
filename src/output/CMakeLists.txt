set(OUPUT_SOURCES 
    "output.cc")
set(OUPUT_HEADERS
    "output.hpp")

add_library(${OUTPUT} STATIC
    ${OUPUT_SOURCES}
    ${OUPUT_HEADERS})

target_include_directories(${OUTPUT} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")