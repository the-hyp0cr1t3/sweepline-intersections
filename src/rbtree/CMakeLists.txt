# rbtree is a header only library, hence an interface is required
add_library(rbtree INTERFACE)

# rbtree_impl.tpp is directly #included by rbtree.tpp, hence not listed here
target_sources(rbtree INTERFACE
  "${CMAKE_SOURCE_DIR}/include/rbtree/iterator.tpp"
  "${CMAKE_SOURCE_DIR}/include/rbtree/rbtree.tpp"
)

target_include_directories(rbtree INTERFACE
  "${CMAKE_SOURCE_DIR}/include/rbtree"
)