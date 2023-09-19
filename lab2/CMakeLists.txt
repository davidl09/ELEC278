cmake_minimum_required(VERSION 3.25)
project(lab2 C)

set(CMAKE_C_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

add_executable(lab2
        list.c
        #main.c
)
