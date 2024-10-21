cmake_minimum_required(VERSION 3.29)
project(miris C)

set(CMAKE_C_STANDARD 11)

add_executable(miris main.c
        node.h
        edge.h
        graph.h
        node.c
        edge.c
        graph.c)
