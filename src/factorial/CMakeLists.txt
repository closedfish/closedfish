set(FAC_SOURCES 
    "factorial.cpp")
set(FAC_HEADERS
    "factorial.hpp")

add_library(${FAC} STATIC
    ${FAC_SOURCES}
    ${FAC_HEADERS})

target_include_directories(${FAC} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${FAC} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${FAC} optimized)
endif()