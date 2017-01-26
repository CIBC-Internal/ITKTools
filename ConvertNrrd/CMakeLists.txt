cmake_minimum_required(VERSION 2.8)

project(ConvertNrrd)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(ConvertNrrd ConvertNrrd.cxx)
target_link_libraries(ConvertNrrd ${ITK_LIBRARIES})
