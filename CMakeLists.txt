cmake_minimum_required(VERSION 2.8.3)
project(ldlidar_stl)

## Compile as C++11
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
message(STATUS "optional:-std=c++11")

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
)

## Get current directory files
#aux_source_directory(./src file_name)
#if(file_name MATCHES "slbf.cpp")
#  add_definitions(-D"USE_SLBF")
 # message("-- #define USE_SLBF")
#endif()

file(GLOB  MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME}  ${MAIN_SRC})
target_link_libraries(${PROJECT_NAME} pthread)


## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/ldlidar.cpp
# )

#############
## Testing ##
#############


