#include <point.hpp>
#include <cmath>

bool geometry::point_t::operator == (const point_t &other) const {
    return std::fabs(x - other.x) < EPS and std::fabs(y - other.y) < EPS;
}