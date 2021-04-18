cmake_minimum_required(VERSION 3.10)

# set the project name
project(CamTest)

SET("OpenCV_DIR" "C:/OpenCV/")

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )



set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# add the executable
add_executable(CamTest main.cpp)




target_link_libraries(CamTest ${OpenCV_LIBS} )
target_link_libraries(CamTest
    ${CMAKE_DL_LIBS}
)


set_property(TARGET CamTest PROPERTY CXX_STANDARD 20)