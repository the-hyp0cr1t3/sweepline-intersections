#include <generators.hpp>

std::vector<geometry::segment_t> generators::gen_oblique_grid(size_t num_horiz, size_t num_verti) {
    std::vector<geometry::segment_t> res;
    size_t cnt = 0;
    const size_t N = 1e6;
    for(size_t i = 0; i < num_horiz; i++) {
        geometry::float_t x1 = -N, y1 = -N + i * N / num_horiz;
        geometry::float_t x2 = N, y2 = y1 + 2 * N;
        res.push_back({ geometry::point_t{ x1, y1 }, geometry::point_t{ x2, y2 }, cnt++ });
    }

    for(size_t i = 0; i < num_verti; i++) {
        geometry::float_t x1 = -N, y1 = N + i * N / num_verti;
        geometry::float_t x2 = N, y2 = y1 - 2 * N;
        res.push_back({ geometry::point_t{ x1, y1 }, geometry::point_t{ x2, y2 }, cnt++ });
    }

    return res;
}