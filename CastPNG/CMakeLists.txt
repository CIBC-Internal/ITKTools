cmake_minimum_required(VERSION 2.8)

project(CastPNG)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(CastPNG CastPNG.cxx)
target_link_libraries(CastPNG ${ITK_LIBRARIES})
