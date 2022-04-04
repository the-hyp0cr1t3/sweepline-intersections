add_library(rbtree INTERFACE)

target_sources(rbtree INTERFACE
    "${CMAKE_SOURCE_DIR}/include/rbtree/iterator.tpp"
    "${CMAKE_SOURCE_DIR}/include/rbtree/rbtree.tpp"
)

target_include_directories(rbtree INTERFACE
    "${CMAKE_SOURCE_DIR}/include/rbtree"
)