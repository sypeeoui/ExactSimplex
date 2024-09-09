# CMakeLists.txt

cmake_minimum_required(VERSION 3.5)

project(ExactSimplex)

## Include the modules

# Eigen
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/eigen)

# tcb/rational
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/rational/include)

## Simplex library
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(Simplex SHARED src/Simplex.cpp)
set_target_properties(Simplex PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Type Option
option(USE_TYPE "Choose the type: INT, DOUBLE, or FLOAT" "DEFAULT")
if (USE_TYPE STREQUAL DOUBLE)
    message("Using double type")
    target_compile_definitions(Simplex PUBLIC USE_DOUBLE)
elseif (USE_TYPE STREQUAL FLOAT)
    message("Using float type")
    target_compile_definitions(Simplex PUBLIC USE_FLOAT)
elseif (USE_TYPE STREQUAL RATIONAL_INT)
    message("Using rational<int> type")
    target_compile_definitions(Simplex PUBLIC USE_RATIONAL_INT)
else()
    message("Using default type (rational<long long>)")
endif()

## Add debug flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

## Add all executables on the mainFolder directory
file(GLOB_RECURSE LT_SOURCES "mainFolder/*.cpp")
foreach(LT_SOURCE ${LT_SOURCES})
    get_filename_component(LT_NAME ${LT_SOURCE} NAME_WE)
    add_executable(${LT_NAME} ${LT_SOURCE})
    target_link_libraries(${LT_NAME} Simplex)
    set_target_properties(${LT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
endforeach()
