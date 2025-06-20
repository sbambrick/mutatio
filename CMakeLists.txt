cmake_minimum_required(VERSION 3.20)
project(mutatio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(ENABLE_TESTING ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Format)
include(Module)
include(Test)

find_package(geographiclib REQUIRED)

if(ENABLE_TESTING)
  find_package(GTest REQUIRED)
endif()

if(ENABLE_COVERAGE)
  include(Coverage)
  set(CMAKE_CXX_FLAGS "-g -fno-inline --coverage" CACHE INTERNAL "")
endif()

if(ENABLE_ASAN)
  add_compile_options(-fsanitize=address)
  add_link_options(-fsanitize=address)
endif()

add_subdirectory(mutatio)
