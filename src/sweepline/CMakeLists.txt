project(sweepline_lib)

add_library(sweepline STATIC
  sweepline.cpp
  event.cpp

  # headers are optional, but won't show up nicely in IDEs unless listed
  "${CMAKE_SOURCE_DIR}/include/sweepline/event.hpp"
  "${CMAKE_SOURCE_DIR}/include/sweepline/sweepline.hpp"
)

# PUBLIC because users of our library will need it too
target_include_directories(sweepline PUBLIC
  "${CMAKE_SOURCE_DIR}/include/sweepline"
)

target_link_libraries(sweepline
  PUBLIC
    geometry
  PRIVATE
    bbst
    fmt::fmt
)