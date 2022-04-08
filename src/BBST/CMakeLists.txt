# bbst is a header only library, hence an interface is required
add_library(bbst INTERFACE)

# red_black_tree_impl.tpp is directly #included by red_black_tree.tpp
target_sources(bbst INTERFACE
  "${CMAKE_SOURCE_DIR}/include/BBST/iterator.tpp"
  "${CMAKE_SOURCE_DIR}/include/BBST/red_black_tree.tpp"
  "${CMAKE_SOURCE_DIR}/include/BBST/red_black_tree_impl.tpp"
)

target_include_directories(bbst INTERFACE
  "${CMAKE_SOURCE_DIR}/include/BBST"
)