cmake_minimum_required(VERSION 2.8)

project (TEST_ServiceDispatcher)

enable_testing()


#Search for check.h
find_path(CHECK_INCLUDE_DIR NAMES check.h)

#Include directories
include_directories(
    ${CHECK_INCLUDE_DIR}
    ../inc
    external/fifo/inc
    )

#Add dependencies to other projects
add_subdirectory("../" FUT)
add_subdirectory("external/fifo")

#Set required libraries
find_library(LIBRARY_SUBUNIT NAMES subunit)

set(MyLIBS check m rt pthread)

if(LIBRARY_SUBUNIT)
  set(MyLIBS ${MyLIBS} ${LIBRARY_SUBUNIT})
endif(LIBRARY_SUBUNIT)

  
#create tests/executables
add_executable(test_ServiceDispatcherQueue  check_ServiceDispatcherQueue.c)
#add_executable(test_ServiceDispatcherRouter check_ServiceDispatcherRouter.c)

target_link_libraries(test_ServiceDispatcherQueue  ServiceDispatcher ${MyLIBS})
#target_link_libraries(test_ServiceDispatcherRouter ServiceDispatcher ${MyLIBS})

add_test(test_ServiceDispatcherQueue  test_ServiceDispatcherQueue)
#add_test(TEST_ServiceDispatcherRouter test_ServiceDispatcherRouter)

