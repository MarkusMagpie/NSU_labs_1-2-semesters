cmake_minimum_required(VERSION 3.5.1)
project(lab7 C)

set(SRC src/main.c src/graph.h src/graph.c src/stack.h src/stack.c)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
