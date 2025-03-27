cmake_minimum_required(VERSION 3.10)
project(MyCppProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create an executable named 'hello' from main.cpp
add_executable(hello main.cpp)
