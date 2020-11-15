cmake_minimum_required(VERSION 3.10)
project(MRI_reconstruction)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    find_package(OpenCV REQUIRED)

    include_directories(${OpenCV_INCLUDE_DIRS})

    add_executable(MRI_reconstruction app/main.cpp)

    target_link_libraries(MRI_reconstruction ${OpenCV_LIBS}
endif()