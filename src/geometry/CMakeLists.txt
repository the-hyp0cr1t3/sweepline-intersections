add_library(geometry STATIC
  point.cpp
  segment.cpp

  # headers are optional, but won't show up nicely in IDEs unless listed
  "${CMAKE_SOURCE_DIR}/include/geometry/constants.hpp"
  "${CMAKE_SOURCE_DIR}/include/geometry/point.hpp"
  "${CMAKE_SOURCE_DIR}/include/geometry/segment.hpp"
)

# PUBLIC because users of our library will need it too
target_include_directories(geometry PUBLIC
  "${CMAKE_SOURCE_DIR}/include/geometry"
)