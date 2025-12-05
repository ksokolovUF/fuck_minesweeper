cmake_minimum_required(VERSION 3.20)
project(the-sweeper)
set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -std=c++20 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# use SFML from distro package manager
find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system audio)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.hpp" "src/*.h")
file(GLOB BASE "*.cpp" "*.hpp" "*.h")

add_executable(the-sweeper
  ${SRC}
  ${BASE}
)

target_link_libraries(the-sweeper PRIVATE
  sfml-system sfml-window sfml-graphics sfml-audio
)
target_compile_features(the-sweeper PRIVATE cxx_std_20)

