#include <generators.hpp>
#include <cmath>

const geometry::float_t PI = acos(-1);

std::vector<geometry::segment_t> generators::gen_origin_star(size_t num_segments) {
    std::vector<geometry::segment_t> res;
    const int r = 1000;

    for(size_t i = 0; i < num_segments; i++) {
        geometry::float_t theta_in_deg = 180 * i / num_segments;
        geometry::float_t theta = theta_in_deg * PI / 180;
        geometry::float_t x1 = r * cos(theta);
        geometry::float_t y1 = r * sin(theta);
        geometry::float_t x2 = -x1;
        geometry::float_t y2 = -y1;
        res.push_back({ geometry::point_t{ x1, y1 }, geometry::point_t{ x2, y2 }, i });
    }

    return res;
}