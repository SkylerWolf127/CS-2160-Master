cmake_minimum_required(VERSION 3.29)
project(PJ02_MemoryDumper_rewrite C)

set(CMAKE_C_STANDARD 11)

add_executable(PJ02_MemoryDumper_rewrite main.c
        memdump.c)
