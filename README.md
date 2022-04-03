# Project

To configure:
```sh
cmake -S . -B build
```

To build:
```sh
cmake --build build
```

To test:
```sh
cmake --build build --target test
```

To build docs (requires Doxygen, output in build/docs/html):
```
cmake --build build --target docs
```

To run:
```sh
./bin/app
```