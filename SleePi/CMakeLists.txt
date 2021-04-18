cmake_minimum_required(VERSION 3.10)

# set the project name
project(SleePI)

SET("OpenCV_DIR" "C:/OpenCV/")

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()
# Set binary folder
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# add libraries
find_package( OpenCV REQUIRED )
add_subdirectory(libraries/dlib-19.22 dlib_build)
find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
# include directories
include_directories(SleePi
 ${OpenCV_INCLUDE_DIRS}
 "static/"
 "include/"
 "src/"
 "libraries/"
 )

# add the executables
add_executable(SleePi
  ${CMAKE_SOURCE_DIR}/src/SleePi.cpp
  ${CMAKE_SOURCE_DIR}/src/play_audio.cpp
  )

# Link libraries
target_link_libraries(SleePi
${OpenCV_LIBS}
 Threads::Threads
 dlib::dlib
 ${CMAKE_DL_LIBS}
 )
# Set c++20 standard
set_property(TARGET SleePi PROPERTY CXX_STANDARD 20)