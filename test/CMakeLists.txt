cmake_minimum_required(VERSION 2.8)
project (TEST_ServiceDispatcher)


#Search for check.h
find_path(CHECK_INCLUDE_DIR NAMES check.h)
include_directories(${CHECK_INCLUDE_DIR})
include_directories(../inc)
include_directories(../external/fifo/inc)


#Set required libraries
find_library(LIBRARY_SUBUNIT NAMES subunit)

set(MyLIBS check m rt pthread)

if(LIBRARY_SUBUNIT)
  set(MyLIBS ${MyLIBS} ${LIBRARY_SUBUNIT})
endif(LIBRARY_SUBUNIT)

  
#create executable
add_executable(test_ServiceDispatcherQueue  check_ServiceDispatcherQueue.c)
#add_executable(test_ServiceDispatcherRouter check_ServiceDispatcherRouter.c)

target_link_libraries(test_ServiceDispatcherQueue  ServiceDispatcher ${MyLIBS})
#target_link_libraries(test_ServiceDispatcherRouter ServiceDispatcher ${MyLIBS})

