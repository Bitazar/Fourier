cmake_minimum_required(VERSION 3.17)

project(FourierImages VERSION 1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(EPSILON_RELEASE_FLAGS "-Wall -Wextra -Werror -pedantic -O3 -fcoroutines")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EPSILON_RELEASE_FLAGS}")

add_executable(${PROJECT_NAME} main.cpp
    ${PROJECT_SOURCE_DIR}/src/FourierImages/Application.cpp
    ${PROJECT_SOURCE_DIR}/src/FourierImages/Fourier.cpp
    ${PROJECT_SOURCE_DIR}/src/FourierImages/FourierDrawer.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/mpgl)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/external/mpgl/include)

target_link_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/external/mpgl/src)

target_link_libraries(${PROJECT_NAME} mpgl fftw3 m)
