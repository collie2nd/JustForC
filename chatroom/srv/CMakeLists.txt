include_directories(${PROJECT_SOURCE_DIR}/srv/include)

set(SERVICR_SRC src/chatsrv.c main.c)

set(CMAKE_LIBRARY_PATH lib/usrmgr.c)

add_library(libusrmgr ${CMAKE_LIBRARY_PATH})

add_executable(service.out ${SERVICR_SRC})

TARGET_LINK_LIBRARIES(service.out pthread libusrmgr)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Release)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/lib)

set_target_properties(libusrmgr PROPERTIES OUTPUT_NAME "usrmgr")