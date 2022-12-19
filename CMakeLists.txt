cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
set(PROJECT_NAME BridgesChecker)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} src/main.cpp)

# add_subdirectory(external/MyLibs)
# target_link_libraries(${PROJECT_NAME} MyLibs)