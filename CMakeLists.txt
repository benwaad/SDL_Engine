cmake_minimum_required(VERSION 3.25)

project(EngineDev)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Hack to work around objective-c bug in cmake, exposed by SDL_image
enable_language(OBJC)

add_subdirectory(engine)
add_subdirectory(base)

add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_image)
