file(GLOB SRCS *.cpp)
target_sources(ShowMySide PUBLIC ${SRCS})

add_subdirectory(GUI)
add_subdirectory(Network)