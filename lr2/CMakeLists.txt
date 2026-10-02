cmake_minimum_required(VERSION 3.10)
project(HashTableLab)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab2 
    main.cpp
    HashTable.h
    HashTable.cpp
)

target_compile_options(lab2 PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

if(APPLE)
    target_compile_options(lab2 PRIVATE 
        -stdlib=libc++
    )
endif()