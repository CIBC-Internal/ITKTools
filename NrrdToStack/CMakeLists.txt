cmake_minimum_required(VERSION 2.8)

project(NrrdToStack)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(NrrdToStack NrrdToStack.cxx)
target_link_libraries(NrrdToStack ${ITK_LIBRARIES})
