cmake_minimum_required(VERSION 3.25)
project(hashtable C)

set(CMAKE_C_STANDARD 17)

add_library(hashmap STATIC hashmap.c)

add_executable(hashtable main.c)

target_link_libraries(hashtable PUBLIC hashmap m)
