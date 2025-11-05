cmake_minimum_required(VERSION 3.18.0 FATAL_ERROR)
project(WebHacker)
cmake_policy(VERSION 3.18.0..3.29.6)

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.20.0")
   # enable RTTI on MSVC
   cmake_policy(SET CMP0117 OLD)
endif()

set(BUILD_MAJOR "0")
set(BUILD_MINOR "1")
set(BUILD_MICRO "0")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(test)
add_subdirectory(docs)