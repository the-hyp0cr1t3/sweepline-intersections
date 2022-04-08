# A sweepline algorithm to find line segment intersections
**Problem:** Given $n$ line segments, efficiently find intersection points in $\mathcal{O}((n + k) log n)$ where $k$ is the number of intersections.

This is a modern C++ implementation of the [Bentley-Ottman algorithm](https://en.wikipedia.org/wiki/Bentley%E2%80%93Ottmann_algorithm) from (almost) scratch because why not.

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

#### Testing `find_intersections()` against various edge cases:
```cmake
cmake --build build --target test
```

#### Stress testing the Red Black tree implementation:
```sh
cd scripts
./stress_rbtree.sh rbtree_test
```

#### Benchmarking against varying input sizes:
```sh
./bin/bench --benchmark_counters_tabular=true
```

```
2022-03-31T04:19:51+05:30
Running ./bench
Run on (12 X 3000 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x6)
  L1 Instruction 32 KiB (x6)
  L2 Unified 512 KiB (x6)
  L3 Unified 4096 KiB (x2)
Load Average: 0.95, 1.85, 1.61
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------------------------
Benchmark                       Time             CPU   Iterations num_intersections num_segments
------------------------------------------------------------------------------------------------
BM_ObliqueGrid/32/16       622496 ns       622008 ns         1167               512           48
BM_ObliqueGrid/128/16     2997460 ns      2993406 ns          234            2.048k          144
BM_ObliqueGrid/512/16    13430223 ns     13408425 ns           52            8.192k          528
BM_ObliqueGrid/32/64      2520555 ns      2518585 ns          276            2.048k           96
BM_ObliqueGrid/128/64    11492670 ns     11479617 ns           63            8.192k          192
BM_ObliqueGrid/512/64    54566964 ns     54510818 ns           13           32.768k          576
BM_ObliqueGrid/32/256    11395191 ns     11383711 ns           57            8.192k          288
BM_ObliqueGrid/128/256   49801564 ns     49742071 ns           14           32.768k          384
BM_ObliqueGrid/512/256  214269060 ns    213970551 ns            3          131.072k          768
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
BM_ObliqueGrid_BigO         96.45 NlgN      96.32 NlgN
BM_ObliqueGrid_RMS              6 %             6 %
------------------------------------------------------------------------------------------------
Benchmark                       Time             CPU   Iterations num_intersections num_segments
------------------------------------------------------------------------------------------------
BM_OriginStar/3              2218 ns         2212 ns       319172                 1            3
BM_OriginStar/2003        1173129 ns      1171431 ns          631                 1       2.003k
BM_OriginStar/4003        2245344 ns      2243113 ns          325                 1       4.003k
BM_OriginStar/6003        3301363 ns      3294699 ns          218                 1       6.003k
BM_OriginStar/8003        4346930 ns      4342962 ns          163                 1       8.003k
-----------------------------------------------------------------
Benchmark                       Time             CPU   Iterations
-----------------------------------------------------------------
BM_OriginStar_BigO          43.34 NlgN      43.28 NlgN
BM_OriginStar_RMS               6 %             6 %
```

## Timing Analysis
View the report [here](./report/report.ipynb).
![complexity_plot](https://user-images.githubusercontent.com/55075129/162203618-d92f48b8-d5b5-4d88-a5f7-57a07a761abc.png)
This empirically proves the relation
$$ T(n, k) = \Theta((n+k)logn) $$

</br>

*This page uses math latex formatting. Download the [extension](https://chrome.google.com/webstore/detail/github-math-display/cgolaobglebjonjiblcjagnpmdmlgmda) to render it.*
