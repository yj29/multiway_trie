cmake_minimum_required(VERSION 3.8)
project(3_waytrie)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c 3_way_trie.h)
add_executable(3_waytrie ${SOURCE_FILES})
