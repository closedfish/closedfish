set(CUI_SOURCES 
    "ConnectUISAI.cpp")
set(CUI_HEADERS
    "ConnectUISAI.h")

add_library(${CUI} STATIC
    ${CUI_SOURCES}
    ${CUI_HEADERS})
    
target_include_directories(${CUI} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${CUI} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${CUI} optimized)
endif()