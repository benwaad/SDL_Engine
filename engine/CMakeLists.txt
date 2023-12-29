cmake_minimum_required(VERSION 3.25)

project(Engine)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC Base SDL3::SDL3 SDL3_image::SDL3_image ${CMAKE_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC Base SDL3::SDL3 SDL3_image::SDL3_image)

