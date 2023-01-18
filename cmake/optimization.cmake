# Enable Release Build
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Enable Parallel Build
if(NOT MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -O2")
endif()

# Enable ccache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# Enable Parallel Test
include(CTest)
set(CTEST_PARALLEL_LEVEL 4)


# Function to create Visual Studio source groups from directories 
function( generate_groups BASE_DIR CURR_DIR )
    string( REPLACE "/" "\\" GROUP_NAME "${CURR_DIR}" )
    source_group( "Source Files\\${GROUP_NAME}" REGULAR_EXPRESSION ${BASE_DIR}/${CURR_DIR}.*\\.\(cpp\|c\|cxx\|cc\) )
    source_group( "Header Files\\${GROUP_NAME}" REGULAR_EXPRESSION ${BASE_DIR}/${CURR_DIR}.*\\.\(hpp\|h\|hxx\|hh\) )
    file( GLOB FILE_LIST RELATIVE ${BASE_DIR} ${BASE_DIR}/${CURR_DIR}/* )
    foreach( FILE ${FILE_LIST} )
        if( IS_DIRECTORY ${BASE_DIR}/${FILE} )
            generate_groups( ${BASE_DIR} ${FILE} )
        endif()
    endforeach()
endfunction()

if( WIN32 )
    set( LCOV_PATHS "C:/LCOV/bin" "C:/Program Files/LCOV/bin" )
else()
    set( LCOV_PATHS "/usr/local/bin" "/usr/bin" )
endif()

if( DEFINED ENV{LCOV_HOME} )
    set( LCOV_HINTS "$ENV{LCOV_HOME}/bin" ${LCOV_HINTS} )
endif()
if( LCOV_HOME )
    set( LCOV_HINTS "${LCOV_HOME}/bin" ${LCOV_HINTS} )
endif()

find_program( LCOV_EXECUTABLE lcov PATHS ${LCOV_PATHS} HINTS ${LCOV_HINTS} )

get_filename_component( LCOV_BASE_DIR ${LCOV_EXECUTABLE} DIRECTORY )

find_program( GENHTML_EXECUTABLE genhtml HINTS ${LCOV_BASE_DIR} NO_DEFAULT_PATH )
#LCOV
if( LCOV_EXECUTABLE )
    find_package( Perl REQUIRED )

    if( DEFINED Perl_FOUND )
        # Get version
        execute_process(
            COMMAND ${PERL_EXECUTABLE} ${LCOV_EXECUTABLE} -v
            OUTPUT_VARIABLE LCOV_VERSION_OUTPUT_VARIABLE
            RESULT_VARIABLE LCOV_VERSION_RESULT_VARIABLE
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE )

        if( NOT LCOV_VERSION_RESULT_VARIABLE )
            string( REGEX REPLACE "^[^:]*: LCOV version (.+)$" "\\1" LCOV_VERSION_STRING ${LCOV_VERSION_OUTPUT_VARIABLE} )
        endif()
    endif()
endif()

include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( LCOV
                                   REQUIRED_VARS LCOV_EXECUTABLE GENHTML_EXECUTABLE
                                   VERSION_VAR LCOV_VERSION_STRING )

mark_as_advanced( LCOV_EXECUTABLE GENHTML_EXECUTABLE )

unset( LCOV_PATHS )
unset( LCOV_BASE_DIR )
unset( LCOV_VERSION_OUTPUT_VARIABLE )
unset( LCOV_VERSION_RESULT_VARIABLE )

if( WIN32 AND NOT MSVC )
    add_definitions( -DWIN32 )
endif()