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
    "switch_main.cc")
set(EXE_HEADERS 
    "switch_main.hpp")
    
add_executable(${EXECUTABLE} 
    ${EXE_SOURCES}
    ${EXE_HEADERS})


target_link_libraries(${EXECUTABLE} PUBLIC 
    ${SC}
    ${GC}
    ${PLAY}
    ${INPUT}    
    ${OUTPUT}
    ${OPENING}
    ${CONAN_CATCH2}
    external/stockfish)
  
