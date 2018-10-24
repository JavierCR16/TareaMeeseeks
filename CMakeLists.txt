cmake_minimum_required(VERSION 3.12)
project(TareaMeeseeks C)

set(CMAKE_C_STANDARD 11)

add_executable(TareaMeeseeks main.c funciones.c threads.c)

TARGET_LINK_LIBRARIES(TareaMeeseeks pthread)