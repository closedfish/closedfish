include(GNUInstallDirs)

# If no build type was specified, set it to Release.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Choose the type of build, options are: None Debug Release."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# If no installation prefix is given manually, install locally.
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE STRING
            "The install location"
            FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(EXE_SOURCES
    "SwitchMain.cpp")
set(EXE_HEADERS 
    "SwitchMain.h")

add_compile_definitions(CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
message("Build type: " ${CMAKE_BUILD_TYPE})
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(DEBUG)
    add_compile_definitions(MODE_CLI=1)
else(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(MODE_CLI=0)
endif(CMAKE_BUILD_TYPE MATCHES Debug)
    
add_executable(${EXECUTABLE} 
    ${EXE_SOURCES}
    ${EXE_HEADERS})

option(MT "enable multiple threading" ON)
if(MT)
# Enable multi-threading
find_package(Threads REQUIRED)

# Link executable with thread library
target_link_libraries(${EXECUTABLE} Threads::Threads)

# Set number of threads for program
set(THREADS 6)

target_compile_definitions(${EXECUTABLE} PRIVATE NUM_THREADS=${THREADS})

endif()

if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${EXECUTABLE} ENABLE ON AS_ERROR OFF)
endif()

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE} PUBLIC 
    ${SC}
    ${GC}
    ${CUI}
    ${PLAY}
    ${INPUT}    
    ${OUTPUT}
    ${OPENING}
    ${BI}
    ${SF}
    ${WRAP}
    ${LOGGER}
    ${ENGINE}
    JsonCpp::JsonCpp
)


