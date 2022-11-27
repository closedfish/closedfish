set(GC_SOURCES 
    "general_main.cc")
set(GC_HEADERS
    "general_main.hpp")

add_library(${GC} STATIC
    ${GC_SOURCES}
    ${GC_HEADERS})

target_include_directories(${GC} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")