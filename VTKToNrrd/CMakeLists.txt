cmake_minimum_required(VERSION 2.8)

project(VTKToNrrd)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(VTKToNrrd VTKToNrrd.cxx)
target_link_libraries(VTKToNrrd ${ITK_LIBRARIES})

add_executable(NrrdToVTK NrrdToVTK.cxx)
target_link_libraries(NrrdToVTK ${ITK_LIBRARIES})
