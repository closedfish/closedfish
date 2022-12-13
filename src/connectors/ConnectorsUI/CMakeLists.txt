set(CUI_SOURCES 
    "ConnectUI-SAI.cc")
set(CUI_HEADERS
    "ConnectUI-SAI.hpp")

add_library(${CUI} STATIC
    ${CUI_SOURCES}
    ${CUI_HEADERS})
    
target_include_directories(${CUI} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${CUI} PUBLIC  
    nlohmann_json
    ${BI})

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${CUI} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${CUI} optimized)
endif()