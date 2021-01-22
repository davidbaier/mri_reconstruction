cmake_minimum_required(VERSION 3.10)
project(MRI_reconstruction)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    find_package(OpenCV REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glfw3 3.3 REQUIRED)

    include_directories(${OpenCV_INCLUDE_DIRS})

    add_executable(MRI_reconstruction app/main.cpp src/MRIFile/Storage.hpp src/ContextWindow/render.hpp src/ContextWindow/controls.hpp)

    target_link_libraries(MRI_reconstruction ${OpenCV_LIBS} ${OPENGL_LIBRARY} glfw GLEW)
endif()