# Copyright 2025 Petr Petrov. All rights reserved.
# License: https://github.com/PetrPPetrov/algopt/blob/main/LICENSE

cmake_minimum_required(VERSION 3.20)

project(algopt)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)
