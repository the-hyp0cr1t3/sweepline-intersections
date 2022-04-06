#include <sweepline/geometry/point.hpp>
#include <cmath>

bool Sweepline::Geometry::point_t::operator == (const point_t &other) const {
    return std::fabs(x - other.x) < EPS and std::fabs(y - other.y) < EPS;
}

// std::ostream &Sweepline::Geometry::operator << (std::ostream &os, const point_t &p) {
//     return os << '(' << p.x << ", " << p.y << ')';
// }