cmake_minimum_required(VERSION 2.8.7)
project(webserver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

find_package(Boost 1.46.0 REQUIRED system)

include_directories(include)
set(SOURCE_FILES src/main.cpp src/server.cpp src/connection.cpp src/reply.cpp src/request.cpp)

add_executable(httpd ${SOURCE_FILES})
target_link_libraries(httpd -lboost_thread ${Boost_LIBRARIES})
