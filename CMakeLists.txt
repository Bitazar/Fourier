cmake_minimum_required(VERSION 3.17)

project(FourierImages VERSION 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} main.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/src)
