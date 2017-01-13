cmake_minimum_required(VERSION 2.8)

project(RGBToGray2D)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(RGBToGray2D RGBToGray2D.cxx)
target_link_libraries(RGBToGray2D ${ITK_LIBRARIES})
