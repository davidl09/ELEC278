cmake_minimum_required(VERSION 3.25)
project(lab6 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab6 main.c)

target_link_libraries(lab6 m)
