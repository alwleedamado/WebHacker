cmake_minimum_required(VERSION 3.16)
project(ts_with_treesitter LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(WebHacker src/main.cpp)
target_link_libraries(WebHacker PRIVATE tree-sitter-typescript)
