cmake_minimum_required(VERSION 3.15)
project(WordSoup)

set(CMAKE_CXX_STANDARD 14)

add_executable(WordSoup main.cpp)

add_subdirectory(google-tests)
