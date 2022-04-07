#!/bin/bash
# Run as follows:
#   cd scripts
#   ./stress_rbtree.sh rbtree_test

# relative path to executables from ./scripts
build_path=../build/tests/red_black_tree

for((i=0;;i++)); do
    # generate random input
    $build_path/generator > $build_path/stress_in

    # pipe rbtree_impl solution to stress_soln.out
    $build_path/$1 $build_path/stress_in > $build_path/stress_soln.out

    # pipe stl_set solution to stress_stl_set.out
    $build_path/stl_set $build_path/stress_in > $build_path/stress_stl_set.out

    # run a diff check to exactly match the two
    diff $build_path/stress_soln.out $build_path/stress_stl_set.out

    # break if not identical
    if [ $? -ne 0 ]; then
        break
    fi

    printf "Ok test ${i}\n"
done