cmake_minimum_required(VERSION 3.16)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CSGUI CsGui REQUIRED)

add_executable(WebHacker src/main.cpp)
target_link_libraries(WebHacker PRIVATE WhCore ${CSGUI_LIBRARIES})
target_include_directories(WebHacker PRIVATE ${CSGUI_INCLUDE_DIRS})