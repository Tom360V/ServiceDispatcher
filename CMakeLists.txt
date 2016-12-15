cmake_minimum_required(VERSION 2.8)

project(ServiceDispatcher)

#Add related projects
add_subdirectory(external/fifo)


#Add include directories
include_directories(inc)
#include_directories(external/fifo/inc)


#Add sources
set(srcs src/ServiceDispatcherQueue.c)
set(srcs src/ServiceDispatcherRouter.c)



#Add Library
add_library(ServiceDispatcher SHARED ${srcs})


#include_directories(inc)

target_link_libraries(ServiceDispatcher FIFO)

#######################################

#if (TEST)
#  set(testsrcs test/test.c)
#  add_executable(fifo_test ${testsrcs})
#  target_link_libraries(fifo_test FIFO)
#endif()


