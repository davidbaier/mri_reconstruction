cmake_minimum_required(VERSION 3.10)
project(MRI_reconstruction)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

add_executable(MRI_reconstruction main.cpp)

target_link_libraries(MRI_reconstruction ${OpenCV_LIBS})