set(BOARD_TEST
    "board_tests")
set(BOARD_TEST_SOURCES
    "test_from_fen.cpp" "test_to_fen.cpp" "test_naive_check_check.cpp")
set(BOARD_TEST_HEADERS 
    "test_from_fen.h" "test_to_fen.h" "test_naive_check_check.h")

add_executable(${BOARD_TEST} ${BOARD_TEST_SOURCES})
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${BOARD_TEST} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(${BOARD_TEST} PUBLIC ${BI})

find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)
catch_discover_tests(${BOARD_TEST})