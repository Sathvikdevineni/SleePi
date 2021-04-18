cmake_minimum_required(VERSION 3.10)

# set the project name
project(Main)

SET("OpenCV_DIR" "C:/OpenCV/")

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()
# Set binary folders
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# add libraries
find_package( OpenCV REQUIRED )
add_subdirectory(../../SleePi/libraries/dlib-19.22 dlib_build)

# include directories
include_directories(Main
 ${OpenCV_INCLUDE_DIRS}
 )

# add the executables
add_executable(Main
  main.cpp
  )

# Link libraries
target_link_libraries(Main
${OpenCV_LIBS}
 dlib::dlib
 ${CMAKE_DL_LIBS}
 )
# Set c++20 standard
set_property(TARGET Main PROPERTY CXX_STANDARD 20)