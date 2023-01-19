set(PLAY_SOURCES 
    "PlayMain.cpp"
    "GeneralRegression.cpp")
set(PLAY_HEADERS
    "PlayMain.h"
    "closenessAI.h"
    "GeneralRegression.h")

add_library(${PLAY} STATIC
    ${PLAY_SOURCES}
    ${PLAY_HEADERS})

find_package(Eigen3 CONFIG REQUIRED)
target_include_directories(${PLAY} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/_deps/eigenlib-src/Eigen"
    "${CMAKE_BINARY_DIR}/configured_files/include")
target_link_libraries(${PLAY} PUBLIC
    Eigen3::Eigen
   )

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${PLAY} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${PLAY} optimized)
endif()