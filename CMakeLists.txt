cmake_minimum_required(VERSION 3.8)
project(jetson_vision)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(NavXTimeSync navXTimeSync/AHRS.cpp)

set(PROJECT_SOURCE_DIR src)

set(PROJECT_BINARY_DIR include)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${OpenCV_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS} navXTimeSync ${PROJECT_BINARY_DIR} gen)

set(SOURCE_FILES gen/pose.pb.h ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/threshholder.cpp
		${PROJECT_BINARY_DIR}/threshholder.h ${PROJECT_SOURCE_DIR}/contour_finder.cpp
		${PROJECT_SOURCE_DIR}/corner_finder.cpp ${PROJECT_BINARY_DIR}/corner_finder.h
		${PROJECT_SOURCE_DIR}/corner_checker.cpp ${PROJECT_BINARY_DIR}/corner_checker.h
		${PROJECT_SOURCE_DIR}/pose_estimator.cpp ${PROJECT_BINARY_DIR}/pose_estimator.h)

add_executable(jetson_vision ${SOURCE_FILES})

target_link_libraries(jetson_vision ${OpenCV_LIBS})
target_link_libraries(jetson_vision ${Protobuf_LIBRARIES})
target_link_libraries(jetson_vision ${CONAN_LIBS})