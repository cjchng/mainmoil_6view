#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.20)
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#find_package(OpenCV REQUIRED)

# Enable C++11
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

set( name nppm-demo )

project(${name})
# Print this project name
message( STATUS "Project name: ${name}" )
#application specific
set(OpenCV_DIR /cjDrbx/fic-092021/cv-cj)
include_directories(${OpenCV_DIR}/include)

#find_package(Boost 1.54 REQUIRED COMPONENTS filesystem)
find_package( OpenCV REQUIRED )
# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message( STATUS "OpenCV library status:" )
message( STATUS "    config: ${OpenCV_DIR}" )
message( STATUS "    version: ${OpenCV_VERSION}" )
message( STATUS "    libraries: ${OpenCV_LIBS}" )
message( STATUS "    include path: ${OpenCV_INCLUDE_DIRS}" )
include_directories( ${OpenCV_INCLUDE_DIRS} )
#include_directories(include util)
#link_directories(${CMAKE_SOURCE_DIR}/../dev-cj/bld-dev)
set(APPS
		apps/nppm/demo.cpp
        apps/nppm/cast.cpp
		apps/util/tools.cpp
		)

#set(DEPENDENCIES libx_moildev.so ${OpenCV_LIBS})
#set(DEPENDENCIES ${OpenCV_LIBS})

add_executable(x_mainmoil apps/nppm/main.cpp ../../configdata.cpp ../../moildev.cpp ${APPS})
set_property(TARGET x_mainmoil PROPERTY CXX_STANDARD 11)
target_link_libraries(x_mainmoil ${OpenCV_LIBS})

