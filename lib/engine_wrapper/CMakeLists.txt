set(WRAP_SOURCES 
    "EngineWrapper.cpp")
set(WRAP_HEADERS
    "EngineWrapper.h")

add_library(${WRAP} STATIC
    ${WRAP_SOURCES}
    ${WRAP_HEADERS})
    
target_include_directories(${WRAP} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${WRAP} PUBLIC ${BI})

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${WRAP} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${WRAP} optimized)
endif()