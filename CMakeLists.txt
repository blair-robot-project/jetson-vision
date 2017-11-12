cmake_minimum_required(VERSION 3.8)
project(jetson_vision)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(PROJECT_SOURCE_DIR src)

set(PROJECT_BINARY_DIR include)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/threshholder.cpp ${PROJECT_BINARY_DIR}/threshholder.h)

add_executable(jetson_vision ${SOURCE_FILES})

target_link_libraries(jetson_vision ${OpenCV_LIBS})