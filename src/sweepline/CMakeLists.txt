project(sweepline_lib)

add_library(sweepline STATIC
  sweepline.cpp
  event.cpp

  # headers are optional, but won't show up nicely in IDEs unless listed
  "${CMAKE_SOURCE_DIR}/include/sweepline/event.hpp"
  "${CMAKE_SOURCE_DIR}/include/sweepline/sweepline.hpp"
  "${CMAKE_SOURCE_DIR}/include/BBST/red_black_tree.tpp"
)

target_include_directories(sweepline
  PUBLIC "${CMAKE_SOURCE_DIR}/include/sweepline"
)

target_link_libraries(sweepline
  PUBLIC
    geometry
    bbst
  PRIVATE
    fmt::fmt
)