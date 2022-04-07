#include <generators.hpp>

std::vector<segment_t> gen_oblique_grid(size_t num_horiz, size_t num_verti) {
    std::vector<segment_t> res;
    size_t cnt = 0;
    const size_t N = 1e6;
    for(size_t i = 0; i < num_horiz; i++) {
        _float_t x1 = -N, y1 = -N + i * N / num_horiz;
        _float_t x2 = N, y2 = y1 + 2 * N;
        res.push_back({ point_t{ x1, y1 }, point_t{ x2, y2 }, cnt++ });
    }

    for(size_t i = 0; i < num_verti; i++) {
        _float_t x1 = -N, y1 = N + i * N / num_verti;
        _float_t x2 = N, y2 = y1 - 2 * N;
        res.push_back({ point_t{ x1, y1 }, point_t{ x2, y2 }, cnt++ });
    }

    return res;
}