cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)

project(ShowMySide)

add_executable(ShowMySide src/main.cpp)

add_subdirectory(src)
add_subdirectory(external)

target_include_directories(ShowMySide PRIVATE include external)
target_link_directories(ShowMySide PUBLIC lib)

target_link_libraries(ShowMySide PUBLIC fltk fltk_images)