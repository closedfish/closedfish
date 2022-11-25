set(GENERATOR_SOURCES 
    "generator.cpp"
    "main.cpp")
set(GENERATOR_HEADERS
    "generator.hpp")

add_library(${GENERATOR} STATIC
    ${GENERATOR_SOURCES}
    ${GENERATOR_HEADERS})
    
target_include_directories(${PLAY} PUBLIC
    "./"
    "${CMAKE_BINARY_DIR}/configured_files/include")