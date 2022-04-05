add_library(geometry STATIC
  event.cpp
  point.cpp
  segment.cpp

  # headers are optional, but won't show up in IDEs unless listed
  "${CMAKE_SOURCE_DIR}/include/sweepline/geometry/event.hpp"
  "${CMAKE_SOURCE_DIR}/include/sweepline/geometry/point.hpp"
  "${CMAKE_SOURCE_DIR}/include/sweepline/geometry/segment.hpp"
)

target_include_directories(geometry PRIVATE
  "${CMAKE_SOURCE_DIR}/include"
)