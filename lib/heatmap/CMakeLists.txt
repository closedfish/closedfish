set(HMP_SOURCES 
    "Heatmap.cpp" "BitOperations.cpp")
set(HMP_HEADERS
    "Heatmap.h" "BitOperations.h")

add_library(${HMP} STATIC
    ${HMP_SOURCES}
    ${HMP_HEADERS})
    
target_include_directories(${HMP} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${HMP} PUBLIC ${BI})

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${HMP} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${HMP} optimized)
endif()