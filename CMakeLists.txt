cmake_minimum_required(VERSION 3.5.0)
project(tmc16xx VERSION 0.1.0 LANGUAGES CXX)

project ("tmc16xx" CXX)
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.h")
add_executable(tmc16xx ${SOURCES})