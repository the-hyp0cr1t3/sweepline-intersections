#include <segment.hpp>
#include <cmath>

namespace sweepline {
    /// A global variable which stores the current X coordinate of the vertical sweepline
    extern geometry::float_t sweeplineX;
}

geometry::float_t geometry::segment_t::eval_y(geometry::float_t x) const {
    return std::fabs(p.x - q.x) < EPS? p.y
                : p.y + (q.y - p.y) * (x - p.x) / (q.x - p.x);
}

bool geometry::segment_t::operator < (const segment_t &other) const {
    return eval_y(sweepline::sweeplineX) < other.eval_y(sweepline::sweeplineX) - EPS;
}

bool geometry::can_intersect_1d(
    geometry::float_t l1, geometry::float_t r1, geometry::float_t l2, geometry::float_t r2
) {
    if(l1 > r1) std::swap(l1, r1);
    if(l2 > r2) std::swap(l2, r2);
    return std::max(l1, l2) <= std::min(r1, r2) + EPS;
}

int geometry::cross_prod(
    const geometry::point_t &a, const geometry::point_t &b, const geometry::point_t &c
) {
    geometry::float_t s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return std::fabs(s) < EPS ? 0 : s > 0 ? +1 : -1;
}

bool geometry::is_intersecting(const segment_t &a, const segment_t &b) {
    return can_intersect_1d(a.p.x, a.q.x, b.p.x, b.q.x)
           and can_intersect_1d(a.p.y, a.q.y, b.p.y, b.q.y)
           and cross_prod(a.p, a.q, b.p) * cross_prod(a.p, a.q, b.q) <= 0
           and cross_prod(b.p, b.q, a.p) * cross_prod(b.p, b.q, a.q) <= 0;
}

geometry::point_t geometry::intersection_point(const segment_t &a, const segment_t &b) {
    geometry::float_t A1 = a.p.y - a.q.y;
    geometry::float_t B1 = a.q.x - a.p.x;
    geometry::float_t C1 = (a.p.y - a.q.y) * (-a.p.x) + (a.q.x - a.p.x) * (-a.p.y);

    geometry::float_t A2 = b.p.y - b.q.y;
    geometry::float_t B2 = b.q.x - b.p.x;
    geometry::float_t C2 = (b.p.y - b.q.y) * (-b.p.x) + (b.q.x - b.p.x) * (-b.p.y);

    return geometry::point_t {(B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1), (C1 * A2 - C2 * A1) / (A1 * B2 - A2 * B1)};
}