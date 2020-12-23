cmake_minimum_required (VERSION 3.8)

project(clw-11)

set(CMAKE_CXX_STANDARD 17)
#[[
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()]]

set(CMAKE_CXX_FLAGS " -fno-stack-protector")

# add_executable(task-any-all-test src/task_any_all/test.cpp)
# add_executable(task-for-each-test src/task_for_each/test.cpp)
# add_executable(task-at-many-test src/task_tuple_utils/test_at_many.cpp)
# add_executable(task-transform-elements-test src/task_tuple_utils/test_transform_elements.cpp)
# add_executable(task-apply-test src/task_tuple_utils/test_apply.cpp)
add_executable(task-join-test src/task_join/test.cpp)