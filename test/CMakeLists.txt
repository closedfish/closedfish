
set(TEST_MAIN 
    "unit_tests")
set(TEST_SOURCES
    "TestMain.cpp")
set(TEST_HEADERS 
    "TestMain.hpp")

add_executable(${TEST_MAIN} ${TEST_SOURCES})
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(${TEST_MAIN} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
target_link_libraries(${TEST_MAIN} PUBLIC
    ${SC}
    ${GC}
    ${PLAY}
    ${INPUT}    
    ${OUTPUT}
    ${OPENING}
    ${FAC})   

enable_testing()