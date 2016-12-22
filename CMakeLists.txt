cmake_minimum_required(VERSION 2.8)

project(ServiceDispatcher)

enable_testing()


#Add related projects
add_subdirectory(test)
add_subdirectory(external/fifo)


#Add include directories
include_directories(inc)
include_directories(external/fifo/inc)


#Add sources
set(srcs src/ServiceDispatcherQueue.c
         src/ServiceDispatcherRouter.c)


#Add Library
add_library(ServiceDispatcher SHARED ${srcs})
target_link_libraries(ServiceDispatcher FIFO)

add_test(TEST_ServiceDispatcherQueue  test/test_ServiceDispatcherQueue)
#add_test(TEST_ServiceDispatcherRouter test/test_ServiceDispatcherRouter)
