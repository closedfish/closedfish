set(OUPUT_SOURCES 
    "output.cpp")
set(OUPUT_HEADERS
    "output.h")

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