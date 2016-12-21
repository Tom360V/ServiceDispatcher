cmake_minimum_required(VERSION 2.8)

project(ServiceDispatcher)

#Add related projects
add_subdirectory(external/fifo)


#Add include directories
include_directories(inc)
include_directories(external/fifo/inc)


#Add sources
set(srcs src/ServiceDispatcherQueue.c)
set(srcs src/ServiceDispatcherRouter.c)


#Add Library
add_library(ServiceDispatcher SHARED ${srcs})
target_link_libraries(ServiceDispatcher FIFO)


#######################################


  #Search for check.h
  FIND_PATH(CHECK_INCLUDE_DIR NAMES check.h)
  FIND_LIBRARY(LIBRARY_CHECK   NAMES check)
  FIND_LIBRARY(LIBRARY_SUBUNIT NAMES subunit)

  set(MY_LIBS  ${LIBRARY_CHECK} libm.so librt.so libpthread.so)


  IF(LIBRARY_SUBUNIT)
    set(MY_LIBS  ${MY_LIBS} ${LIBRARY_SUBUNIT}) 
  ENDIF(LIBRARY_SUBUNIT)

#  message("********************************")
#  message("My LIBS:: " ${MY_LIBS})
#  message("********************************")

  include_directories(${CHECK_INCLUDE_DIR})

  add_executable(test_serviceDispatcherQueue  test/check_ServiceDispatcherQueue.c)
#  add_executable(test_serviceDispatcherRouter test/check_ServiceDispatcherRouter.c)

  target_link_libraries(test_serviceDispatcherQueue  ServiceDispatcher  ${MY_LIBS})
#  target_link_libraries(test_serviceDispatcherRouter ServiceDispatcher ${MY_LIBS})

  add_test(ServiceDispatcherQueue  test_serviceDispatcherQueue)
#  add_test(ServiceDispatcherRouter test_serviceDispatcherRouter)



