set(BOARD_TEST
    "board_tests")
set(BOARD_TEST_SOURCES
    "Tests/test_Board_Implementation.cpp")
set(BOARD_TEST_HEADERS 
    "Tests/test_Board_Implementation.h")

add_executable(${BOARD_TEST} ${BOARD_TEST_SOURCES})
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${BOARD_TEST} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(${BOARD_TEST} PUBLIC ${BI})

find_package(Catch2 CONFIG REQUIRED)
include(CTest)
include(Catch)
catch_discover_tests(${BOARD_TEST})