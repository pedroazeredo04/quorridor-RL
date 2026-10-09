cmake_minimum_required(VERSION 3.15)
project(quorridor-RL CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_library(quorridor_lib src/gameState.cpp src/display.cpp)

target_include_directories(quorridor_lib PUBLIC include)


add_executable(QUORRIDOR apps/gui.cpp)
target_link_libraries(QUORRIDOR PRIVATE quorridor_lib raylib)
