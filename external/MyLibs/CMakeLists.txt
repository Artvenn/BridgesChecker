cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
set(PROJECT_NAME MyLibs)
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} STATIC 
    ./include/Arr.h
    ./include/File.h
    ./include/Memory.h
    ./include/Str.h
    ./include/System.h
    ./include/TypeAliases.h

    ./src/File.cpp
    ./src/Memory.cpp
    ./src/Str.cpp
    ./src/System.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ./include)