/**
 * @file main.cpp
 * @author the-hyp0cr1t3
 * @brief Main source file
 * @date 2022-03-25
 */
#include <fstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <fmt/format.h>

#define OUTPUT_PATH "outfile.txt"
#define LOGFILE_PATH "logfile.txt"    // only logs in DEBUG mode

#include <utils.hpp>
#include <sweepline.hpp>

using Sweepline::Geometry::point_t;
using Sweepline::Geometry::segment_t;
using Sweepline::intersection_t;

/**
 * @brief Entry point
 *
 * @param argc The number of commandline arguments
 * @param argv A list of commandline arguments
 * @return nothing
 */
int main(int argc, char *argv[]) {

    if(argc < 2 or argc > 3) {
        std::cerr << "Invalid args; Usage ./main <fname> <output-path(optional)>" << std::endl;
        std::cerr << "eg: ./main sample_test.txt" << std::endl;
        exit(EXIT_FAILURE);
    }

    // creating input file stream using argv[1] as fname
    std::ifstream fin;
    std::string fname = argv[1];
    utils::open_file(fin, fname);

    size_t n;       // number of input segments
    fin >> n;

    std::vector<segment_t> segments;
    segments.reserve(n);

    // getting input
    for(size_t i = 0; i < n; i++) {
        float_t x1, y1, x2, y2;
        fin >> x1 >> y1 >> x2 >> y2;

        if(std::make_pair(x1, y1) > std::make_pair(x2, y2))
            std::swap(x1, x2), std::swap(y1, y2);

        segments.emplace_back(segment_t{ point_t{ x1, y1 }, point_t{ x2, y2 }, i });
    }


    // finding intersections
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    std::vector<intersection_t> result = Sweepline::find_intersections(segments);

    std::chrono::high_resolution_clock::duration total_runtime = std::chrono::high_resolution_clock::now() - start;


    std::string output_path = argc == 3? argv[2] : OUTPUT_PATH;
    std::ofstream fout(output_path, std::ios::trunc);

#ifndef NDEBUG
    std::ofstream ferr(LOGFILE_PATH, std::ios::app);
    ferr << result.size() << " intersections found!" << std::endl;
#endif

    fout << result.size() << std::endl;
    for(intersection_t it: result) {
        fout << it.pt.x << ' ' << it.pt.y << '\t';
        for(int idx: it.segments)
            fout << ' ' << idx + 1;
        fout << std::endl;

#ifndef NDEBUG
        ferr << it << std::endl;
#endif

    }

    std::cout << "Total runtime: " << std::fixed << std::setprecision(5)
        << std::chrono::duration<double, std::milli>(total_runtime).count() << " ms" << std::endl;

    fout << std::chrono::duration<double, std::milli>(total_runtime).count() << " ms" << std::endl;
}