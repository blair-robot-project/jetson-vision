cmake_minimum_required(VERSION 3.8)
project(jetson_vision)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(jetson_vision ${SOURCE_FILES})

target_link_libraries(jetson_vision ${OpenCV_LIBS})