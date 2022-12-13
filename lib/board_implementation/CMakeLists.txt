set(BI_SOURCES 
    "CFBoard.cpp")
set(BI_HEADERS
    "CFBoard.h")

add_library(${BI} STATIC
    ${BI_SOURCES}
    ${BI_HEADERS})
    
target_include_directories(${BI} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${BI} PUBLIC  
    nlohmann_json)

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${BI} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${BI} optimized)
endif()