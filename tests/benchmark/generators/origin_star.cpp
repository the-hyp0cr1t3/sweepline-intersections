#include <generators.hpp>
#include <cmath>

const _float_t PI = acos(-1);

std::vector<segment_t> gen_origin_star(size_t num_segments) {
    std::vector<segment_t> res;
    const int r = 1000;

    for(size_t i = 0; i < num_segments; i++) {
        _float_t theta_in_deg = 180 * i / num_segments;
        _float_t theta = theta_in_deg * PI / 180;
        _float_t x1 = r * cos(theta);
        _float_t y1 = r * sin(theta);
        _float_t x2 = -x1;
        _float_t y2 = -y1;
        res.push_back({ point_t{ x1, y1 }, point_t{ x2, y2 }, i });
    }

    return res;
}