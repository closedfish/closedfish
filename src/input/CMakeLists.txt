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

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${INPUT} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${INPUT} optimized)
endif()