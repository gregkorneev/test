cmake_minimum_required(VERSION 3.10)
project(converter_demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем include/
include_directories(${CMAKE_SOURCE_DIR}/include)

# ВКЛЮЧАЕМ оба .cpp в цель app
add_executable(app
    src/main.cpp
    src/converter.cpp
)
