cmake_minimum_required(VERSION 3.1.0)
project(raytracer_sw)

# C++ options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall -pedantic")
set(CMAKE_BUILD_TYPE RELEASE)
#set(CMAKE_BUILD_TYPE DEBUG)

# Libraries
find_package(SDL2 REQUIRED)

# Source files
file(GLOB SRC_FILES "src/*.cpp")

# Target
add_executable(raytracer_sw ${SRC_FILES})
target_link_libraries(raytracer_sw ${SDL2_LIBRARIES} "-pthread")
target_include_directories(raytracer_sw PRIVATE ${SDL2_DIRS})
