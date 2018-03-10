cmake_minimum_required(VERSION 3.9)
project(SetOne C)

set(CMAKE_C_STANDARD 99)

add_executable(SetOne main.c table_manager.h table_manager.c table_manager_static.h table_manager_static.c main_dynamic.c)