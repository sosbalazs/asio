cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

# set the project name
project(common)

# include directories
include_directories(include)
include_directories("c:/asio-1.20.0/include/")

set(SOURCES 
src/net_client.cpp
src/net_connection.cpp 
# do it later src/net_message.cpp
src/net_server.cpp
src/net_tsqueue.cpp)

# add the executable
#add_executable(common ${SOURCES})
add_library(common STATIC ${SOURCES})

# asio needs these libraries
if(MINGW)
  target_link_libraries(common ws2_32)
  # only the server needs this library:
  target_link_libraries(common wsock32)
endif()