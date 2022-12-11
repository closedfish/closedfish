set(PLAY_SOURCES 
    "PlayMain.cpp")
set(PLAY_HEADERS
    "PlayMain.hpp"
    "closenessAI.h"
    "general_regression.h")

add_library(${PLAY} STATIC
    ${PLAY_SOURCES}
    ${PLAY_HEADERS})
    
target_include_directories(${PLAY} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(${PLAY} PUBLIC  
    nlohmann_json
    Eigen3::Eigen)

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${PLAY} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${PLAY} optimized)
endif()