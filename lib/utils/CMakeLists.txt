set(UTILS_SOURCES 
    "utils.cpp" "logger.cpp")
set(UTILS_HEADERS
    "utils.h")

add_library(${UTILS} STATIC
    ${UTILS_SOURCES}
    ${UTILS_HEADERS})
    
target_include_directories(${UTILS} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${UTILS} PUBLIC ${BI} ${SF})

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${UTILS} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${UTILS} optimized)
endif()