cmake_minimum_required(VERSION 2.8)

project(VTKTest)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(VTKTest VTKTest.cxx)
target_link_libraries(VTKTest ${ITK_LIBRARIES}}
