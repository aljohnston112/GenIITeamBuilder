cmake_minimum_required(VERSION 3.27)
project(GenIITeamBuilder)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} main.cpp
        MyHandler.h)

if(CMAKE_BUILD_TYPE MATCHES Debug)
   add_compile_options(-O0 -fno-inline)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE )
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

