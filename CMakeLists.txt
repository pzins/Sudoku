project(Sudoku)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11")
   SET(CMAKE_BUILD_TYPE RelWithDebInfo)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
aux_source_directory(. SRC_LIST)


include_directories(../tess_install/include)

file(
    GLOB_RECURSE
    files
    *.h
    *.cpp
)
SET(CMAKE_INSTALL_RPATH "/home/pierre/Dev/tess_install/lib")

find_package( OpenCV REQUIRED )
add_executable( Sudoku ${files})
target_link_libraries( Sudoku ${OpenCV_LIBS} lept)
target_link_libraries( Sudoku /home/pierre/Dev/tess_install/lib/libtesseract.so)
