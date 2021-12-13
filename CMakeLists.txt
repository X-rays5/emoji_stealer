cmake_minimum_required(VERSION 3.20)
project(emoji_stealer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(vendor/cpr)
add_subdirectory(vendor/fmt)

add_executable(emoji_stealer src/main.cpp src/api.hpp)

target_include_directories(emoji_stealer PRIVATE vendor/rapidjson/include)

target_link_libraries(emoji_stealer PRIVATE cpr)
target_link_libraries(emoji_stealer PRIVATE fmt)