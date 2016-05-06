cmake_minimum_required(VERSION 2.8)

project(StackToNrrd)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(StackToNrrd StackToNrrd.cxx)
target_link_libraries(StackToNrrd ${ITK_LIBRARIES})
