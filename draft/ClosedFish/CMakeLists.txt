# CMakeList.txt : CMake project for ClosedFishCmake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ClosedFishCmake")

set(CMAKE_CXX_COMPILER_ID "MSVC")

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )


# Add source to this project's executable.
add_executable (ClosedFishCmake "ClosedFish.cpp" "ChessBot.cpp"
"HarrisCorner.cpp" "class_def.cpp" "stringtable.rc")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers) 

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ClosedFishCmake)

target_link_libraries(ClosedFishCmake ${OpenCV_LIBS})

#target_include_directories(ClosedFishCmake PRIVATE C:/OpenCV/include/opencv2)

# Set Subsystem to Windows
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

#add_compile_definitions(_WIN32_WINNT = 0x0601)

#target_link_libraries(your_target_name windowscodecs.lib)

install(TARGETS ClosedFishCmake)
