cmake_minimum_required(VERSION 2.8.3)


find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})


include_directories(
  include
)

add_executable(segmentation segmentation.cpp)
target_link_libraries(segmentation /usr/lib/libpcl_kdtree.so)
target_link_libraries(segmentation ${PCL_LIBRARIES})
