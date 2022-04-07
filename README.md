# Project

To configure:
```sh
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
```

To build:
```sh
cmake --build build --config Release
```

To test:
```sh
cmake --build build --target test
```

To build docs (requires Doxygen, output in docs/html):
```
cmake --build build --target docs
```

To run:
```sh
./bin/app
```

To run benchmark:
```sh
./bin/bench --benchmark_counters_tabular=true
```