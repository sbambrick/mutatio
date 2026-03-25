cmake_minimum_required(VERSION 3.20)
project(PackageTest CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(mutatio REQUIRED)

add_executable(example src/example.cc)
target_link_libraries(example PRIVATE mutatio::mutatio)
