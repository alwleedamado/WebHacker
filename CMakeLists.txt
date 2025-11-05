cmake_minimum_required(VERSION 3.16)
project(tsanalyzer_cpp)

set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(PkgConfig REQUIRED)

add_subdirectory(src/parser)
add_executable(tsanalyzer
  src/main.cpp
  src/Analyzer.cpp
)
target_link_libraries(tsanalyzer PRIVATE ts_parser)
target_include_directories(tsanalyzer PRIVATE include)
target_include_directories(tsanalyzer PRIVATE ${TS_INCLUDE_DIRS})
target_link_libraries(tsanalyzer PRIVATE Qt6::Widgets ${TS_LIBRARIES})
# Add nlohmann_json::nlohmann_json if available, else add single header include
