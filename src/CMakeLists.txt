add_library(sdq SHARED ServiceDispatcherQueue.c)
target_link_libraries(sdq fifo)

add_library(sdr SHARED ServiceDispatcherRouter.c)
