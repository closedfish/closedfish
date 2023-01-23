set(INT_TEST
    "integration_tests")
set(INT_TEST_SOURCES
    "TestChess.cpp" "TestChessHelper.cpp" "TestRandomMove.cpp")
set(INT_TEST_HEADERS
    "TestChessHelper.h" "TestRandomMove.h")

add_compile_definitions(CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
add_executable(${INT_TEST} ${INT_TEST_SOURCES} ${INT_TEST_HEADERS})
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${INT_TEST} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(${INT_TEST} PUBLIC 
    ${SC}
    ${GC}
    ${CUI}
    ${PLAY}
    ${INPUT}    
    ${OUTPUT}
    ${OPENING}
    ${BI}
    ${SF}
    ${WRAP}
    ${LOGGER}
    ${ENGINE}
)

find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)
catch_discover_tests(${INT_TEST})