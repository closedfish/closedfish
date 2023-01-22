set(GC_SOURCES 
    "ClosedfishConnect.cpp")
set(GC_HEADERS
    "ClosedfishConnect.h")

add_library(${GC} STATIC
    ${GC_SOURCES}
    ${GC_HEADERS})

target_include_directories(${GC} PUBLIC 
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(${GC} PUBLIC ${BI} ${WRAP})

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${GC} ENABLE ON AS_ERROR OFF)
endif()

if(${ENABLE_LTO})
    target_enable_lto(${GC} optimized)
endif()