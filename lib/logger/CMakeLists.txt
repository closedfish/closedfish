set(LOGGER_SOURCES 
    "logger.cpp")
set(LOGGER_HEADERS
    "logger.h")

add_library(${LOGGER} STATIC
    ${LOGGER_SOURCES}
    ${LOGGER_HEADERS})
    
target_include_directories(${LOGGER} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${LOGGER} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${LOGGER} optimized)
endif()