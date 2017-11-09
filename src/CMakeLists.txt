
cmake_minimum_required(VERSION 2.8)

project(LandmarkWarping3)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(LandmarkWarping3 MACOSX_BUNDLE LandmarkWarping3.cxx)

if( "${ITK_VERSION_MAJOR}" LESS 4 )
  target_link_libraries(LandmarkWarping3 ${ITK_LIBRARIES})
else( "${ITK_VERSION_MAJOR}" LESS 4 )
  target_link_libraries(LandmarkWarping3 ${ITK_LIBRARIES})
endif( "${ITK_VERSION_MAJOR}" LESS 4 )

