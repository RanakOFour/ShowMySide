file(GLOB SRCES *.cpp)
target_sources(ShowMySide PUBLIC ${SRCES})

add_subdirectory(GUI)
add_subdirectory(Network)