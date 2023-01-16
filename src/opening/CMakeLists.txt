set(OPENING_SOURCES 
    "OpeningMain.cpp")
add_library(${OPENING} STATIC
    ${OPENING_SOURCES}
    ${OPENING_HEADERS})
    
target_include_directories(${OPENING} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")
find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(${OPENING} PRIVATE 
    JsonCpp::JsonCpp)
if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${OPENING} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${OPENING} optimized)
endif()