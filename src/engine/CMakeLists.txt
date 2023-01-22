set(ENGINE_SOURCES 
    "SwitchEngine.cpp")
set(ENGINE_HEADERS
    "SwitchEngine.h")

add_library(${ENGINE} STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS})
    
target_include_directories(${ENGINE} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${ENGINE} PUBLIC ${BI} ${WRAP} ${SF} ${UTILS} ${SC} ${GC})

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${ENGINE} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${ENGINE} optimized)
endif()