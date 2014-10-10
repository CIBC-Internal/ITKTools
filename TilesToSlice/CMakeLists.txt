cmake_minimum_required(VERSION 2.8)

project(Tiles)

find_package(ITK REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(OpenMP REQUIRED)

include(${ITK_USE_FILE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

add_executable(TilesToSliceRGB TilesToSliceRGB.cxx)
target_link_libraries(TilesToSliceRGB ${ITK_LIBRARIES} ${Boost_LIBRARIES})

add_executable(TilesToSubSliceRGB TilesToSubSliceRGB.cxx)
target_link_libraries(TilesToSubSliceRGB ${ITK_LIBRARIES} ${Boost_LIBRARIES})
