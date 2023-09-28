cmake_minimum_required(VERSION 3.10)
project(cowsay)
set (CMAKE_CXX_STANDARD 17)

set(SOURCE main.cpp)
set(HEADERS PipeReader.h)

add_executable(cowsay ${SOURCE} ${HEADERS})