/**
 * @file main.cpp
 * @author the-hyp0cr1t3
 * @brief Main app file
 * @date 2022-03-25
 */
#include <iostream>
#include <vector>
#include <chrono>
#include <fmt/format.h>
#include <fmt/color.h>

#include <utils.hpp>
#include <sweepline.hpp>

/**
 * @brief Gathers input from stdin or a file and returns a vector of segments
 *
 * Input must be a description of \f$ n \f$ line segments in the following format.
 * The first line must contain a single integer \f$ n \f$ — the number of line segments.
 * Each of the next \f$ n \f$ lines must contain four real numbers \f$ p_x \f$, \f$ p_y \f$, \f$ q_x \f$, \f$ q_y \f$
 * — the coordinates of the endpoints \f$ p \f$ and \f$ q \f$ of the corresponding line segment.
 *
 * @pre `std::cin` must be redirected to appropriate file stream before function call
 * @return `std::vector<geometry::segment_t>` A vector of segments from the input
 */
std::vector<geometry::segment_t> input() {
    size_t n;       // number of input segments
    std::cin >> n;

    std::vector<geometry::segment_t> segments;
    segments.reserve(n);

    for(size_t i = 0; i < n; i++) {
        float_t x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        if(std::make_pair(x1, y1) > std::make_pair(x2, y2))
            std::swap(x1, x2), std::swap(y1, y2);

        segments.emplace_back(geometry::segment_t{
                                geometry::point_t{ x1, y1 },
                                geometry::point_t{ x2, y2 },
                                i });
    }

    return segments;
}

/**
 * @brief Prints a vector of intersections to stdout or a file
 *
 * The format of output is as follows.
 * The first line will contain a single integer \f$ m \f$ — the number of intersections.
 * Each of the next \f$ m \f$ lines will contain two real numbers \f$ x \f$ and \f$ y \f$,
 * folllowed by two or more integers
 * — the coordinates of the corresponding intersection point
 * and the indices (1-based) of the segments in the input that intersect at this point.
 *
 * @pre `std::cout` must be redirected to appropriate file stream before function call
 *
 * @param result A vector of intersection points to print
 * @param enable_color Commandline boolean flag which enables or disables printing in color
 */
void output(const std::vector<sweepline::intersection_t> &result, bool enable_color) {
    std::cout << fmt::format("  {}\n", result.size());
    // std::cout << fmt::format("{}\n", result.size());
    for(auto it: result) {
        std::cout << format_col(enable_color,
            fmt::emphasis::faint | fg(fmt::color::medium_aquamarine),
            "  ({:.3f}, {:.3f})  ", it.pt.x, it.pt.y
            // "{:.10f} {:.10f} ", it.pt.x, it.pt.y
        );

        bool fst = true;
        for(int idx: it.segments) {
            std::cout << (fst? "" : ", ")
            // std::cout << (fst? "" : " ")
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
 * **Usage** `./app [options]`
 *
 * **Example** `./app --verbose -i sample_test.txt -nc --outputf ~/outfile.txt`
 *
 * Flag             |                                   Description                                         |
 * :--------------: | :------------------------------------------------------------------------------------ |
 * `-h --help`     	| shows help message and exits [default: false]                                         |
 * `-v --version`  	| prints version information and exits [default: false]                                 |
 * `-i --inputf`   	| specify the input file                                                                |
 * `-o --outputf`  	| specify the output file                                                               |
 * `-l --logf`     	| specify the log file                                                                  |
 * `-V --verbose`  	| write useful debug statements that describe the state at every stage [default: false] |
 * `-nc --nocolor` 	| disable color printing [default: false]                                               |
 *
 * @param argc The number of commandline arguments
 * @param argv A list of commandline arguments
 * @return nothing
 */
int main(int argc, char *argv[]) {

    // parsing command line arguments and redirecting input/output streams to specified files
    auto params = utils::parse_args_and_redirect_streams(argc, argv);

    // reading input
    std::vector<geometry::segment_t> segments = input();


    // finding intersections
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

      std::vector<sweepline::intersection_t> result = sweepline::find_intersections(segments, params.verbose, params.enable_color);

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