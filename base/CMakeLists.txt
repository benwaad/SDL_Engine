cmake_minimum_required(VERSION 3.27)

project(Base)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES defines.h base.cpp memory/allocator.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/)
