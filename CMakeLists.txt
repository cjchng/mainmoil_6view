#  minimum required cmake version: 3.1.0
cmake_minimum_required(VERSION 3.1.0)
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(MoilExample)
find_package(OpenCV REQUIRED)
include_directories( include )
link_directories(${CMAKE_SOURCE_DIR}/lib)
set(APPS apps/car6view/car6view.cpp apps/measure/measure.cpp apps/stitch180/stitch180.cpp )
set(DEPENDENCIES libmoildev.a ${OpenCV_LIBS})

add_executable(mainmoil main.cpp ${APPS})
set_property(TARGET mainmoil PROPERTY CXX_STANDARD 11)
target_link_libraries(mainmoil ${DEPENDENCIES})

install(
	TARGETS mainmoil RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
