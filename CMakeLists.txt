cmake_minimum_required(VERSION 3.30)
project(Algorithms_Math_DataStructures)

file(GLOB_RECURSE SOURCES  "src/*.cpp" "src/*.h")
add_executable(main ${SOURCES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${SOURCES})