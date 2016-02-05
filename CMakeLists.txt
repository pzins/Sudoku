project(Sudoku)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11")
   SET(CMAKE_BUILD_TYPE RelWithDebInfo)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
aux_source_directory(. SRC_LIST)



set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt5.5.0/5.5/gcc_64)
find_package(Qt5Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
qt5_wrap_ui(ui_files mainwindow.ui)



include_directories(../tess_install/include)

file(
    GLOB_RECURSE
    files
    *.h
    *.cpp
    *.ui
)
SET(CMAKE_INSTALL_RPATH "/home/pierre/Dev/tess_install/lib")

find_package( OpenCV REQUIRED )
add_executable( Sudoku ${files})
target_link_libraries( Sudoku ${OpenCV_LIBS} lept)
target_link_libraries( Sudoku /home/pierre/Dev/tess_install/lib/libtesseract.so)
target_link_libraries(Sudoku Qt5::Widgets)

