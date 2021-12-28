cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set the project name
project(SimpleServer)

# include directories
#include_directories(include)
include_directories("c:/Users/Balazs/Documents/Workspace/asio/common/include/")
include_directories("c:/asio-1.20.0/include/")

set(SOURCES 
src/SimpleServer.cpp)

# add the executable
add_executable(SimpleServer ${SOURCES})

# asio needs these libraries
if(MINGW)
  target_link_libraries(SimpleServer ws2_32)
  # only the server needs this library:
  target_link_libraries(SimpleServer wsock32)
endif()