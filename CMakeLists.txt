cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(ShowMySide)

add_executable(ShowMySide src/main.cpp)

add_subdirectory(src)
add_subdirectory(Pugixml)

target_include_directories(ShowMySide PRIVATE include)
#target_link_directories(ShowMySide PRIVATE lib)

target_link_libraries(ShowMySide PRIVATE fltk fltk_images)