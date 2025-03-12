cmake_minimum_required(VERSION 3.20)

project(ShowMySide)


add_executable(ShowMySide main.cpp)

add_subdirectory(Source)
add_subdirectory(Pugixml)
find_package(FLTK CONFIG REQUIRED)
include_directories(${FLTK_INCLUDE_DIRS})

target_include_directories(ShowMySide PUBLIC Headers)
target_link_libraries(ShowMySide PUBLIC fltk fltk_forms fltk_gl fltk_images)