cmake_minimum_required(VERSION 3.10)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# Add the executable and source files
add_executable(raytracer src/main.cpp src/stb_image_write_wrapper.cpp)
