cmake_minimum_required(VERSION 3.1...3.14)
project( Memory_manager )

find_package(GLEW REQUIRED)
find_package(Boost COMPONENTS thread REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)

include_directories(./include)

ADD_EXECUTABLE(demo src/MemoryManager.cpp src/Application.cpp)
target_link_libraries(demo GL GLU GLEW glfw ${Boost_LIBRARIES} )

