cmake_minimum_required(VERSION 3.8)

project(clw-02)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif ()

set(CMAKE_CXX_FLAGS "-fsanitize=address -O0")
add_executable(matrices matrices.cpp main.cpp)
