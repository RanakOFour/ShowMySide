cmake_minimum_required(VERSION 3.15)

project(ShowMySide VERSION 0.1)



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "./src/*.cpp")

add_executable(ShowMySide "${MY_SOURCES}")

target_include_directories(ShowMySide PUBLIC "./include")

target_link_libraries(ShowMySide FLTK)