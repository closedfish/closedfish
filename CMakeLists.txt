#Overall settings
cmake_minimum_required(VERSION 3.16)

project(ChessSwitchAI VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

#Enable unit testing
option(ENABLE_TESTING "Whether to enable the unit testing build" ON)

#Defining Variables

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(SC StockfishConectors)
set(GC GeneralConnectors)
set(PLAY PlayingRoutine)
set(INPUT ReceiveInputFromUI)
set(OUTPUT SendOutputToUI)
set(OPENING DeceideOverOpening)

set(EXECUTABLE Executable)

# We attempt to use ccache to speed up the build.
find_program(CCACHE_FOUND "ccache")
if(CCACHE_FOUND)
    message(STATUS "Using ccache for building.")
    set_property(GLOBAL PROPERTY LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

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

#Help to include Cmake-based libraries
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(AddGitSubmodule)

#Include Doxygen module
include(Docs)

#Include Conan Libraries
#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

#Includes subdirectories
add_subdirectory(src)
add_subdirectory(config)
add_subdirectory(lib)


#Enable unit testing
option(ENABLE_TESTING "Whether to enable the unit testing build" ON)

#Check if there is need to compile unit test
if(ENABLE_TESTING)
    add_subdirectory(test)
else()
    message("W/o exe. unit")
endif()

#Check if there is need to compile the executable
option(COMPILE_EXECUTABLE "Whether to compile the executable" ON)
if(COMPILE_EXECUTABLE)
    add_subdirectory(app)
else()
    message("W/o exe. compiling")
endif()
