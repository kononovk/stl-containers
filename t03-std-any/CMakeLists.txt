cmake_minimum_required (VERSION 2.8)

project(hw3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Werror -Wextra-semi -Wvla-extension -Wdeprecated-copy")
set(CMAKE_CXX_FLAGS "-fsanitize=address")


add_executable(hw3 smoke_test.cpp)
