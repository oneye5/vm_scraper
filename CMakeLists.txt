cmake_minimum_required(VERSION 3.28)
project(paksaveGetter)

set(CMAKE_CXX_STANDARD 17)

add_executable(paksaveGetter main.cpp
        inputHandler.h
        CharToKeyCode.h)
