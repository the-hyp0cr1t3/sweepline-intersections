# A sweepline algorithm to find line segment intersections
**Problem:** Given $n$ line segments, efficiently find intersection points in $\mathcal{O}((n + k) log n)$ where $k$ is the number of intersections.

This is a modern C++ implementation of the [Bentley-Ottman algorithm](https://en.wikipedia.org/wiki/Bentley%E2%80%93Ottmann_algorithm) from (almost) scratch because why not.

## File structure

```
.
├── docs
│   └── Doxyfile
├── examples
│   ├── edge_cases
│   │   ├── butterfly.txt
│   │   ├── coordinate_axes_1.txt
│   │   ├── ...
│   │   ├── ..
│   │   └── vertical_parallel.txt
│   └── big cases
│       └── ...
├── include
│   ├── constants.hpp
│   ├── event.hpp
│   ├── intersection.hpp
│   ├── iterator.tpp
│   ├── point.hpp
│   ├── red_black_tree.tpp
│   ├── red_black_tree_impl.tpp
│   ├── segment.hpp
│   └── utils.hpp
├── report
│   ├── benchmark.csv
│   ├── report.html
│   └── report.ipynb
├── src
│   ├── event.cpp
│   ├── intersection.cpp
│   ├── main.cpp
│   ├── point.cpp
│   ├── segment.cpp
│   └── utils.cpp
├── tests
│   ├── benchmark
│   │   ├── generators
│   │   │   ├── oblique_grid.cpp
│   │   │   └── origin_star.cpp
│   │   ├── include
│   │   │   └── generators.hpp
│   │   └── benchmark.cpp
│   ├── red_black_tree
│   │   ├── generator.cpp
│   │   ├── normalize.cpp
│   │   ├── red_black_tree.cpp
│   │   └── stl_set.cpp
│   ├── stress_rbtree.sh
│   └── verify_edge_cases.sh
├── CMakeLists.txt
├── requirements.txt
├── run.py
└── README.md
```

## Install Dependencies
To build the project you must have [CMake](https://cmake.org/install/) installed.

To install python dependencies
```sh
pip install -r requirements.txt
```

## Build Project
#### Configure:
```cmake
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
```
#### Build:
```cmake
cmake --build build --config Release
```

## Build Documentation
To build the documentation you must have [Doxygen](https://github.com/doxygen/doxygen) installed.

```cmake
cmake --build build --target docs
```

Output will be in `docs/html`.

## Usage

```bash
./bin/app [options]
```
#### Options:
```brainfuck
-h --help     	shows help message and exits [default: false]
-v --version  	prints version information and exits [default: false]
-i --inputf   	specify the input file
-o --outputf  	specify the output file
-l --logf     	specify the log file
-V --verbose  	write useful debug statements that describe the state at every stage [default: false]
-nc --nocolor 	disable color printing [default: false]
```
#### Examples:
```
./bin/app
./bin/app --verbose -i sample_test.txt -nc --outputf ~/outfile.txt
```

**Note:** `--inputf` may also be relative to `./data`.

### Input Format
Input must contain the description of the $n$ line segments in the following format (either from `stdin` or a file — provide the path along with the `--inputf` flag).

The first line must contain a single integer $n$ — the number of line segments. Each of the next $n$ lines must contain four real numbers $p_x$, $p_y$, $q_x$, $q_y$ — the coordinates of the endpoints $p$ and $q$ of a line segment.

> :warning: Two line segments which coincide with each other either partially or in whole have infinitely many points of intersection. This implementation assumes the input does not have any such cases.

### Output Format
The output will contain the description of the $m$ intersection points corresponding to the $n$ line segments in the following format (either to `stdout` or a file — provide the path along with the `--outputf` flag).

The first line will contain a single integer $m$ — the number of intersection points. Each of the next $m$ lines will contain two real numbers $x$ and $y$, followed by two or more integers — the coordinates of the corresponding intersection point and the indices of the segments in the input (1-based) that intersect at this point.

> :warning: This implementation is precise upto 9 decimal places, after which accuracy starts dropping.

Additionally, if the `--verbose` flag is specified, the program will write useful debug statements that describe its state at every stage (either to `stderr` or a log file — provide the path along with the `--logf` flag).

The program uses [{fmt}](https://github.com/fmtlib/fmt) to produce colored output on the terminal. Use the `--nocolor` flag to disable color printing when required since ANSI escape sequences end up being written to files as plain text.

## Visualization
Python script to run the app against some input and plot a graph with [matplotlib](https://matplotlib.org/) using the output.

```sh
cd scripts
python3 run.py [input_file_path]
```

**Note:** `input_file_path` may also be relative to `./data`.

![Visualization example output](https://user-images.githubusercontent.com/55075129/160944227-342b2220-27e9-4dbd-96b2-29ab7833bb6c.png)

**Note:** Plotting the graph is the bottleneck for large inputs.


## Testing
This project uses [GoogleTest](https://github.com/google/googletest) for its unit tests and [GoogleBenchmark](https://github.com/google/benchmark) for benchmarking.

#### Testing `find_intersections()` against various small edge cases:
```cmake
cmake --build build --target test
```

#### Benchmarking against varying input sizes:
```sh
./bin/bench --benchmark_counters_tabular=true
```

#### Stress testing the Red Black tree implementation:
```sh
cd scripts
./stress_rbtree.sh rbtree_test
```