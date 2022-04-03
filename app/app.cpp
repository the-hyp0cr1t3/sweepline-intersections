#include <project_name/lib.hpp>
#include <fmt/format.h>

int main(int argc, char *argv[]) {
    if(argc == 1) {
        fmt::print(stderr, "Invalid usage");
        exit(EXIT_FAILURE);
    }

#ifndef NDEBUG
    fmt::print("Debug mode.\n");
#else
    fmt::print("Release mode.\n");
#endif

    fmt::print("Hello world {}!", 42);
}