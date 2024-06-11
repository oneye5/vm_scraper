cmake_minimum_required(VERSION 3.28)
project(input_putter)

set(CMAKE_CXX_STANDARD 17)

add_executable(input_putter main.cpp
        automation.h
        CharToKeyCode.h)

