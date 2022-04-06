project(sweepline_lib)

add_subdirectory(geometry)

add_library(sweepline STATIC sweepline.cpp)

# PUBLIC because users of our library will need it too
target_include_directories(sweepline PUBLIC
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_SOURCE_DIR}/include/rbtree"
)

target_link_libraries(sweepline PUBLIC geometry PRIVATE rbtree fmt::fmt)