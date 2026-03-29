cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)

project(ShowMySide)

add_executable(ShowMySide src/main.cpp)

add_subdirectory(src)
add_subdirectory(extern/pugixml)

target_include_directories(ShowMySide PRIVATE include extern extern/pugixml)
target_link_directories(ShowMySide PRIVATE lib)

target_link_libraries(ShowMySide PRIVATE fltk fltk_images fltk_forms png X11 Xrender Xft fontconfig)