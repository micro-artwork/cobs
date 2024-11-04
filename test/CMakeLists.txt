cmake_minimum_required(VERSION 3.20.0)

project(test)

add_executable(${PROJECT_NAME} ../cobs.c test.c)
