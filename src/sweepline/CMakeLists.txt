# project(sweepline_lib)

# add_subdirectory(geometry)

# add_library(sweepline STATIC sweepline.cpp)

# target_include_directories(sweepline PUBLIC
#     "${CMAKE_SOURCE_DIR}/include/sweepline"
# )

# target_link_libraries(sweepline PUBLIC geometry rbtree)

project(sweepline_lib)

# add_subdirectory(geometry)

add_library(sweepline STATIC
    sweepline.cpp
    geometry/event.cpp
    geometry/point.cpp
    geometry/segment.cpp
)

target_include_directories(sweepline PUBLIC
    "${CMAKE_SOURCE_DIR}/include/sweepline"
    "${CMAKE_SOURCE_DIR}/include/sweepline/geometry"
)

target_link_libraries(sweepline PUBLIC rbtree)