cmake_minimum_required(VERSION 2.8.3)
project(ldlidar_stl)

set(VAL  0)
if(${VAL} EQUAL 1)
  set(CMAKE_BUILD_TYPE "Release") 
  set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -Wall")
  #set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11 -O3 -Wall")
  message(STATUS "Mode: Release")
  message(STATUS "optional:-std=c++11")
else()
  set(CMAKE_BUILD_TYPE "Debug") 
  set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -Wall -Wextra -Wpedantic -g2 -ggdb")
  #set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -std=c++11 -O2 -Wall -Wextra -Wpedantic -g2 -ggdb")
  message(STATUS "Mode: Debug")
  message(STATUS "optional:-std=c++11")
endif()

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/include
)

file(GLOB  LDLIDAR_DRIVER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/ldlidar_driver/*.cpp)

add_executable(${PROJECT_NAME}  ${CMAKE_CURRENT_SOURCE_DIR}/src/linux_demo/demo.cpp ${LDLIDAR_DRIVER_SRC})
target_link_libraries(${PROJECT_NAME} pthread)
