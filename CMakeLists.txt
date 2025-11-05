cmake_minimum_required(VERSION 3.16)
project(ts_with_treesitter LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(test)
add_subdirectory(docs)