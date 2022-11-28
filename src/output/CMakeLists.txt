set(OUPUT_SOURCES 
    "Output.cc")
set(OUPUT_HEADERS
    "Output.hpp")

add_library(${OUTPUT} STATIC
    ${OUPUT_SOURCES}
    ${OUPUT_HEADERS})

target_include_directories(${OUTPUT} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${OUTPUT} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${OUTPUT} optimized)
endif()