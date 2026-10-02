cmake_minimum_required(VERSION 3.10)
project(Graph)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab3
    main.cpp
    graph.cpp
    graph.hpp
)

target_compile_options(lab3 PRIVATE 
    -Wall 
    -Wextra 
    -Wpedantic
)

if(APPLE)
    target_compile_options(lab3 PRIVATE 
        -stdlib=libc++
    )
endif()