set(GENERATOR_SOURCES 
    "generator.cpp"
    "main.cpp")
set(GENERATOR_HEADERS
    "generator.hpp")

add_library(${GENERATOR} STATIC
    ${GENERATOR_SOURCES}
    ${GENERATOR_HEADERS})

if(${ENABLE_LTO})
    target_enable_lto(${GENERATOR} optimized)
endif()