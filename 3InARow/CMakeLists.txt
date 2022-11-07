cmake_minimum_required(VERSION 3.23)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
find_package(SFML 2.5.1 COMPONENTS graphics audio)

add_executable(3InARow main.cpp)

target_link_libraries(3InARow sfml-graphics sfml-audio)