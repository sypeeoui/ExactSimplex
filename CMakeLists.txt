# CMakeLists.txt

cmake_minimum_required(VERSION 3.5)

project(ExactSimplex)

## Include the modules

# Eigen
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/eigen)

# tcb/rational
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/rational/include)

## Add all executables that start with lt_*
file(GLOB_RECURSE LT_SOURCES "src/lt_*.cpp")
foreach(LT_SOURCE ${LT_SOURCES})
    get_filename_component(LT_NAME ${LT_SOURCE} NAME_WE)
    add_executable(${LT_NAME} ${LT_SOURCE})
endforeach()

#### Main executable
# TODO