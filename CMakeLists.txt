cmake_minimum_required(VERSION 3.24)
project(red-black-binary-search-tree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(232_07_Lab_115
        bst.h
        spy.h
        testBST.cpp
        testBST.h
        testSpy.h
        unitTest.h)
