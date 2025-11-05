
pkg_check_modules(TS tree-sitter REQUIRED)
pkg_check_modules(TSPARSER tree-sitter-typescript REQUIRED)

add_library(WhCore src/Analyzer.cpp)

target_include_directories(WhCore PRIVATE include)
target_include_directories(WhCore PUBLIC ${TS_INCLUDE_DIRS} ${TSPARSER_INCLUDE_DIRS})
target_link_libraries(WhCore PUBLIC ${TS_LIBRARIES} ${TSPARSER_LIBRARIES})
