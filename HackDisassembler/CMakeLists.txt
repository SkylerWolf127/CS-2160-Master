cmake_minimum_required(VERSION 3.29)
project(HackDisassembler C)

set(CMAKE_C_STANDARD 11)

add_library(HackDisassembler STATIC library.c)
