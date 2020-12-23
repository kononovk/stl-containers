cmake_minimum_required (VERSION 3.8)

project(clw-04)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -fsanitize=address)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wextra-semi")

add_executable(smoke_test smoke_test.cpp)
# add_executable(bignum bignum.cpp)
