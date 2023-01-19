set(DFS1P_SOURCES 
    "DFS1P.cpp")
set(DFS1P_HEADERS
    "DFS1P.h")

add_library(${DFS1P} STATIC
    ${DFS1P_SOURCES}
    ${DFS1P_HEADERS})
    
target_include_directories(${DFS1P} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${DFS1P} PUBLIC ${BI} ${WRAP} ${HMP} ${WEAKP})

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${DFS1P} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${DFS1P} optimized)
endif()