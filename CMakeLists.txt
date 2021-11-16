cmake_minimum_required(VERSION 3.10)
project(cpp-json-api)

set(CMAKE_CXX_STANDARD 17)

find_package (Threads REQUIRED)

add_executable(cppjson main.cpp)
target_link_libraries(cppjson Threads::Threads)