cmake_minimum_required(VERSION 3.20)

project(ShowMySide VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "./src/*.cpp")

set(FLTK_DIR "./fltk/build")

find_package(FLTK 1.4 REQUIRED CONFIG)

add_executable(ShowMySide "${MY_SOURCES}")

target_include_directories(ShowMySide PUBLIC "./include" "${PROJECT_SOURCE_DIR}/fltk/build")

target_link_libraries(ShowMySide WS2_32 fltk::fltk fltk::images fltk::png fltk::forms)