cmake_minimum_required(VERSION 2.8)

project(PadImage)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(PadImage PadImage.cxx)
target_link_libraries(PadImage ${ITK_LIBRARIES})
