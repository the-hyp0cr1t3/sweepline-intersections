/**
 * @file main.cpp
 * @author the-hyp0cr1t3
 * @brief Main source file
 * @date 2022-03-25
 */
#include <iostream>
#include <vector>
#include <chrono>
#include <fmt/format.h>
#include <fmt/color.h>

#include <utils.hpp>
#include <sweepline.hpp>

using Sweepline::Geometry::point_t;
using Sweepline::Geometry::segment_t;
using Sweepline::intersection_t;

std::vector<segment_t> input() {
    size_t n;       // number of input segments
    std::cin >> n;

    std::vector<segment_t> segments;
    segments.reserve(n);

    for(size_t i = 0; i < n; i++) {
        float_t x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        if(std::make_pair(x1, y1) > std::make_pair(x2, y2))
            std::swap(x1, x2), std::swap(y1, y2);

        segments.emplace_back(segment_t{ point_t{ x1, y1 }, point_t{ x2, y2 }, i });
    }

    return segments;
}

void output(const std::vector<intersection_t> &result, bool enable_color) {
    std::cout << fmt::format("  {}\n", result.size());
    for(auto it: result) {
        std::cout << format_col(enable_color,
            fmt::emphasis::faint | fg(fmt::color::medium_aquamarine),
            "  ({:.3f}, {:.3f})  ", it.pt.x, it.pt.y
        );

        bool fst = true;
        for(int idx: it.segments) {
            std::cout << (fst? "" : ", ")
                << format_col(enable_color,
                        fmt::emphasis::faint | fg(fmt::color::khaki), "{}", idx + 1);
            fst = false;
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

/**
 * @brief Entry point
 *
 * @param argc The number of commandline arguments
 * @param argv A list of commandline arguments
 * @return nothing
 */
int main(int argc, char *argv[]) {

    // parsing command line arguments and redirecting input/output streams to specified files
    auto params = utils::parse_args_and_redirect_streams(argc, argv);

    // reading input
    std::vector<segment_t> segments = input();


    // finding intersections
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

      std::vector<intersection_t> result = Sweepline::find_intersections(segments, params.verbose, params.enable_color);

    std::chrono::high_resolution_clock::duration total_runtime = std::chrono::high_resolution_clock::now() - start;


    // n and k values
    fmt::print("  {} = {}\n  {} = {}\n\n",
        format_col(params.enable_color, fmt::emphasis::bold | fg(fmt::color::orange), "num_segments (n)     "),
        segments.size(),
        format_col(params.enable_color, fmt::emphasis::bold | fg(fmt::color::orange), "num_intersections (k)"),
        result.size()
    );

    // writing output
    output(result, params.enable_color);

    // time taken
    fmt::print("  {} {}\n",
        format_col(params.enable_color, fg(fmt::color::red) | fmt::emphasis::bold, "Total runtime:"),
        format_col(params.enable_color, fmt::emphasis::blink, "{:5f} ms",
            std::chrono::duration<double, std::milli>(total_runtime).count())
    );
}