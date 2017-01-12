cmake_minimum_required(VERSION 2.8)

project(RGBTIFFToGrayMHA)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(RGBTIFFToGrayMHA RGBTIFFToGrayMHA.cxx)
target_link_libraries(RGBTIFFToGrayMHA ${ITK_LIBRARIES})
