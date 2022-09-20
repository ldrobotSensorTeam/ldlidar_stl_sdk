cmake_minimum_required(VERSION 2.8.3)
project(ldlidar_stl)

if(${CMAKE_BUILD_TYPE} MATCHES  "Release")
  #set(CMAKE_BUILD_TYPE "Release") 
  set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -Wall")
  #set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall")
  message(STATUS "Mode: Release")
  message(STATUS "optional:-std=c++11 -Wall")
else()
  #set(CMAKE_BUILD_TYPE "Debug") 
  set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -Wall -Wextra -Wpedantic -g2 -ggdb")
  #set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O2 -Wall -Wextra -Wpedantic -g2 -ggdb")
  message(STATUS "Mode: Debug")
  message(STATUS "optional:-std=c++11 -Wall -Wextra -Wpedantic -g2 -ggdb")
endif()

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/include/core/
  ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/include/dataprocess/
  ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/include/filter/
  ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/include/logger/
  ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/include/networkcom/
  ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/include/serialcom/
)

file(GLOB  LDLIDAR_DRI_CORE ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/src/core/*.cpp)
file(GLOB  LDLIDAR_DRI_DATARPC ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/src/dataprocess/*.cpp)
file(GLOB  LDLIDAR_DRI_FILTER ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/src/filter/*.cpp)
file(GLOB  LDLIDAR_DRI_LOGGER ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/src/logger/*.cpp)
file(GLOB  LDLIDAR_DRI_NETWORK ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/src/networkcom/*.cpp)
file(GLOB  LDLIDAR_DRI_SERIAL ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/src/serialcom/*.cpp)

add_executable(${PROJECT_NAME}_node ${CMAKE_CURRENT_SOURCE_DIR}/src/linux_demo/demo.cpp)

add_library(ldlidar_driver_static STATIC 
  ${LDLIDAR_DRI_CORE} 
  ${LDLIDAR_DRI_DATARPC} 
  ${LDLIDAR_DRI_FILTER} 
  ${LDLIDAR_DRI_LOGGER} 
  ${LDLIDAR_DRI_NETWORK} 
  ${LDLIDAR_DRI_SERIAL}
)

add_library(ldlidar_driver_shared SHARED 
  ${LDLIDAR_DRI_CORE} 
  ${LDLIDAR_DRI_DATARPC} 
  ${LDLIDAR_DRI_FILTER} 
  ${LDLIDAR_DRI_LOGGER} 
  ${LDLIDAR_DRI_NETWORK} 
  ${LDLIDAR_DRI_SERIAL}
)

# rename library name
set_target_properties (ldlidar_driver_static PROPERTIES OUTPUT_NAME "ldlidar_driver")
set_target_properties (ldlidar_driver_shared PROPERTIES OUTPUT_NAME "ldlidar_driver")

# binary file link to library
target_link_libraries(${PROJECT_NAME}_node ldlidar_driver_static pthread)

###########
## Install ##
###########

INSTALL(TARGETS ldlidar_driver_static ldlidar_driver_shared
  ARCHIVE DESTINATION lib/ldlidar_driver
  LIBRARY DESTINATION share/ldlidar_driver
)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/ldlidar_driver/include
  DESTINATION include/ldlidar_driver
)
