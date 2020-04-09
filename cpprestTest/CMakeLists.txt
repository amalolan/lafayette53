cmake_minimum_required(VERSION 3.7)

project(cpprestTest)

find_package(cpprestsdk REQUIRED)

add_executable(main main.cpp server.cpp server.h)

target_link_libraries(main PRIVATE cpprestsdk::cpprest)
