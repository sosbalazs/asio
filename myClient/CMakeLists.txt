cmake_minimum_required(VERSION 3.22)


set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
# for scoped_lock
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set the project name
project(myClient)

# include directories
include_directories(include)
include_directories("c:/asio-1.20.0/include/")

set(SOURCES src/main.cpp src/Message.cpp src/Connector.cpp src/Client.cpp)

# add the executable
add_executable(myClient ${SOURCES})
if(MINGW)
  target_link_libraries(myClient ws2_32)
endif()