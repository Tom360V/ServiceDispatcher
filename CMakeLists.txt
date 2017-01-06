cmake_minimum_required(VERSION 2.8)

project(ServiceDispatcher)

#Add include directories
include_directories(inc)

#Add sources
set(srcs src/ServiceDispatcherQueue.c
         src/ServiceDispatcherRouter.c)

#Add Library
add_library(ServiceDispatcher SHARED ${srcs})
target_link_libraries(ServiceDispatcher FIFO)
