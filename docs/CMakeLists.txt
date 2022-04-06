set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
set(DOXYGEN_SHOW_NAMESPACES YES)
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/docs")
set(DOXYGEN_USE_MATHJAX YES)
set(DOXYGEN_EXTENSION_MAPPING "${DOXYGEN_EXTENSION_MAPPING}" ".in=C++" ".tpp=C++")

doxygen_add_docs(docs
  "${CMAKE_SOURCE_DIR}/app/main.cpp"
  "${CMAKE_SOURCE_DIR}/include/sweepline"
  "${CMAKE_SOURCE_DIR}/include/rbtree/iterator.tpp"
  "${CMAKE_SOURCE_DIR}/include/rbtree/rbtree.tpp"
  "${CMAKE_SOURCE_DIR}/include/utils/utils.hpp.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/mainpage.md"
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/include"
  COMMENT "Generate HTML documentation"
)