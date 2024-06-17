cmake_minimum_required(VERSION 3.5.1)
project(lab6-3 C)

set(HEADERS src/Node.h src/Search.h)
set(SRC src/main.c src/Node.c src/Search.c)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
