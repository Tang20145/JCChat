cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")

project(JCChat)

add_subdirectory(src)