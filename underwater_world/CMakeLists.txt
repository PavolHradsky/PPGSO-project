cmake_minimum_required(VERSION 3.23)
project(underwater_world)

set(CMAKE_CXX_STANDARD 14)

add_executable(underwater_world src/main.cpp src/Water.cpp src/Water.h src/Bubble.cpp src/Bubble.h src/Fish.cpp src/Fish.h src/Boat.cpp src/Boat.h)
