cmake_minimum_required(VERSION 3.22)

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
if(MINGW)
  target_link_libraries(SimpleServer ws2_32)
endif()