cmake_minimum_required(VERSION 2.8)

project(Tools)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(RawToPNG RawToPNG.cxx)
target_link_libraries(RawToPNG ${ITK_LIBRARIES})

add_executable(TilesToSliceRGB TilesToSliceRGB.cxx)
target_link_libraries(TilesToSliceRGB ${ITK_LIBRARIES})
