
#Include unit testing sources
if (ENABLE_TESTING)
    set(TEST_MAIN 
        "unit_tests")
    set(TEST_SOURCES
        "test_main.cc")
    set(TEST_HEADERS 
        "test_main.hpp")

    add_executable(${TEST_MAIN} ${TEST_SOURCES})
    target_link_libraries(${TEST_MAIN} PUBLIC
        ${SC}
        ${GC}
        ${PLAY}
        ${INPUT}    
        ${OUTPUT}
        ${OPENING}
        ${CONAN_CATCH2})   
endif()

