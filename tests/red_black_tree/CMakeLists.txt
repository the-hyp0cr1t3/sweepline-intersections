## Stress testing Red Black tree implementation

# red_black_tree driver code
add_executable(rbtree_test red_black_tree.cpp)
target_link_libraries(rbtree_test PRIVATE rbtree)

# C++ STL std::set driver code
add_executable(stl_set stl_set.cpp)

# generator for inputs
add_executable(generator generator.cpp)