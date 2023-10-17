cmake_minimum_required(VERSION 3.25)
project(fifo C)

set(CMAKE_C_STANDARD 17)

add_executable(fifo main.c
        impl_fifo.h)
