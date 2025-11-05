cmake_minimum_required(VERSION 3.16)

pkg_check_modules(TS tree-sitter REQUIRED)
pkg_check_modules(TSPARSER tree-sitter-typescript REQUIRED)

add_library(WhCore src/Analyzer.cpp)

target_include_directories(WhCore PRIVATE include)
target_include_directories(WhCore PRIVATE ${TS_INCLUDE_DIRS} ${TSPARSER_INCLUDE_DIRS})
target_link_libraries(WhCore PRIVATE ${TS_LIBRARIES} ${TSPARSER_LIBRARIES})
