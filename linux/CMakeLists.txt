cmake_minimum_required(VERSION 3.5.0)
project(tmcm16xx VERSION 0.1.0 LANGUAGES CXX)

project ("tmcm16xx" CXX)
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp" "${CMAKE_SOURCE_DIR}/*.h")
add_executable(tmcm16xx ${SOURCES})