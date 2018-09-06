cmake_minimum_required(VERSION 3.5)
project(AncientGrammar)

set(CMAKE_CXX_STANDARD 14)

add_library(AncientGrammar src/verb.cpp src/utf8proc/utf8proc.c include/verb.h include/json.h src/utils.cpp include/utils.h)

add_executable(Test src/test.cpp)
target_link_libraries(Test ${CMAKE_BINARY_DIR}/libAncientGrammar.a)
add_dependencies(Test AncientGrammar)