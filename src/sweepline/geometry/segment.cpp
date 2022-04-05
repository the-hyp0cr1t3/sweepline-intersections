#include <segment.hpp>
#include <cmath>

using SGfloat_t = Sweepline::Geometry::float_t;

SGfloat_t Sweepline::Geometry::segment_t::eval_y(SGfloat_t x) const {
    return std::fabs(p.x - q.x) < EPS? p.y
                : p.y + (q.y - p.y) * (x - p.x) / (q.x - p.x);
}

bool Sweepline::Geometry::segment_t::operator < (const segment_t &other) const {
    return eval_y(sweepline_X) < other.eval_y(sweepline_X) - EPS;
}

bool Sweepline::Geometry::can_intersect_1d(
    SGfloat_t l1, SGfloat_t r1, SGfloat_t l2, SGfloat_t r2
) {
    if(l1 > r1) std::swap(l1, r1);
    if(l2 > r2) std::swap(l2, r2);
    return std::max(l1, l2) <= std::min(r1, r2) + EPS;
}

int Sweepline::Geometry::cross_prod(
    const point_t &a, const point_t &b, const point_t &c
) {
    SGfloat_t s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return std::fabs(s) < EPS ? 0 : s > 0 ? +1 : -1;
}

bool Sweepline::Geometry::is_intersecting(const segment_t &a, const segment_t &b) {
    return can_intersect_1d(a.p.x, a.q.x, b.p.x, b.q.x)
           and can_intersect_1d(a.p.y, a.q.y, b.p.y, b.q.y)
           and cross_prod(a.p, a.q, b.p) * cross_prod(a.p, a.q, b.q) <= 0
           and cross_prod(b.p, b.q, a.p) * cross_prod(b.p, b.q, a.q) <= 0;
}

Sweepline::Geometry::point_t Sweepline::Geometry::intersection_point(const segment_t &a, const segment_t &b) {
    SGfloat_t A1 = a.p.y - a.q.y;
    SGfloat_t B1 = a.q.x - a.p.x;
    SGfloat_t C1 = (a.p.y - a.q.y) * (-a.p.x) + (a.q.x - a.p.x) * (-a.p.y);

    SGfloat_t A2 = b.p.y - b.q.y;
    SGfloat_t B2 = b.q.x - b.p.x;
    SGfloat_t C2 = (b.p.y - b.q.y) * (-b.p.x) + (b.q.x - b.p.x) * (-b.p.y);

    return point_t {(B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1), (C1 * A2 - C2 * A1) / (A1 * B2 - A2 * B1)};
}

std::ostream &Sweepline::Geometry::operator << (std::ostream &os, const Sweepline::Geometry::segment_t &s) {
    return os << "{ " << s.p << ", " << s.q << ", id=" << s.seg_id + 1 << " }";
}