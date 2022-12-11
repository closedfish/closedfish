set(PLAY_SOURCES 
    "PlayMain.cpp")
set(PLAY_HEADERS
    "PlayMain.hpp")

add_library(${PLAY} STATIC
    ${PLAY_SOURCES}
    ${PLAY_HEADERS})
    
target_include_directories(${PLAY} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${PLAY} PUBLIC  
    nlohmann_json)

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${PLAY} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${PLAY} optimized)
endif()