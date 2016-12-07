project(ServiceDispatcher)

cmake_minimum_required(VERSION 2.8)

include_directories(inc)
include_directories(external/fifo/inc)

add_subdirectory(external/fifo)
add_subdirectory(src)
