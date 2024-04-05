cmake_minimum_required(VERSION 3.10)

project(JCChat_Server)

set(CMAKE_CXX_STANDARD 11)

include_directories("./thirdparty/single_include/nlohmann")

#编译项目
aux_source_directory(./src SRC_LIST)

file(GLOB_RECURSE HEADERS *.hpp *.h *.ini)

add_executable(${PROJECT_NAME} ${HEADERS} ${SRC_LIST})

#使用json库

find_package(nlohmann_json 3.2.0 REQUIRED )

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
