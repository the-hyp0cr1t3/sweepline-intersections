add_executable(bench
  benchmark.cpp
  generators/oblique_grid.cpp
  generators/origin_star.cpp

  include/generators.hpp
)

target_include_directories(bench PRIVATE include)

target_link_libraries(bench PRIVATE benchmark::benchmark sweepline)

set_target_properties(bench
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
