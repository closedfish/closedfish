#Overall settings
cmake_minimum_required(VERSION 3.16)

project(ChessSwitchAI VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

#Optimizations
option(OPTI "Whether to use the optimizations for the code" ON)
if(OPTI)
include(cmake/optimization.cmake)
else()
    message("No optimizations")
endif()

#Compile Features
option(CF "Enable compile features" ON)
if(CF)
include(cmake/CXXCompileFeatures.cmake)
else()
    message("No compile features")
endif()

#Enable warnings
option(ENABLE_WARNINGS      "Enable to add warnings to a target."   ON)

#Options
option(ENABLE_CLANG_TIDY    "Enable to add clang tidy."             OFF)
option(ENABLE_CPPCHECK      "Enable to add cppcheck."               OFF)
option(ENABLE_LTO           "Enable to add Link Time Optimization." OFF)
option(ENABLE_CCACHE        "Enable to add Ccache."                 OFF)

#Defining Variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(SC StockfishConectors)
set(GC GeneralConnectors)
set(CUI ConnectorsFromUItoSwicthAI)
set(PLAY PlayingRoutine)
set(INPUT ReceiveInputFromUI)
set(OUTPUT SendOutputToUI)
set(OPENING DeceideOverOpening)
set(BI BoardImplementation)
set(FAC FactorialImplementation)
set(SF StockfishSource)
set(LOGGER CustomLogger)
set(WRAP EngineWrapper)
set(ENGINE SwitchEngine)

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
#import Eigen
#add_git_submodule(external/Eigen)

# #add Catch2 with FetchContent
# Include(FetchContent)

# FetchContent_Declare(
#   Catch2
#   GIT_REPOSITORY https://github.com/catchorg/Catch2
#   GIT_TAG        v3.2.0 # or a later release
#   GIT_SHALLOW TRUE
# )

#import VCPKG
option(USE_VCPKG "Whether to use VCPCKG" ON)
if(USE_VCPKG)
    message("Using VCPKG")
    include(${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake)
    find_package(eigen3)
endif()
#inlude warnings
include(Warnings)

#include option's cmake files
include(StaticAnalyzers)
include(LTO)
include(Ccache)

#Include Doxygen module
find_program(DOXYGEN_FOUND "doxygen")
if(DOXYGEN_FOUND)
	message("Using Doxygen")
	include(Docs)
else(DOXYGEN_FOUND)
	message("Doxygen not found")
endif(DOXYGEN_FOUND)

#Includes subdirectories

add_subdirectory(src)
add_subdirectory(config)
add_subdirectory(lib)
add_subdirectory(external)

#Enable unit testing
option(ENABLE_TESTING "Whether to enable the unit testing build" ON)

#Check if there is need to compile unit test
if(ENABLE_TESTING)
    enable_testing()
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

