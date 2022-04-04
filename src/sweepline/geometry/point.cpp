#include <point.hpp>
#include <cmath>

namespace Sweepline::Geometry {

bool point_t::operator == (const point_t &other) const {
    return std::fabs(x - other.x) < EPS and std::fabs(y - other.y) < EPS;
}

std::ostream &operator << (std::ostream &os, const point_t &p) {
    return os << '(' << p.x << ", " << p.y << ')';
}

} // namespace Sweepline::Geometry