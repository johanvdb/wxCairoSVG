cmake_minimum_required(VERSION 2.6)
project("wxcairosvg")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/build/)

find_package(wxWidgets COMPONENTS net adv xml core base)
find_path(CAIRO_INCLUDE_DIR cairo.h)

include(${wxWidgets_USE_FILE})
include_directories(include)
include_directories(${CAIRO_INCLUDE_DIR})

file(GLOB_RECURSE MY_SOURCES src/*.cpp)
add_library(wxcairosvg ${MY_SOURCES})
