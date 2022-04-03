#include <point.hpp>

bool point_t::operator == (const point_t &other) const {
    return abss(x - other.x) < EPS and abss(y - other.y) < EPS;
}

std::ostream &operator << (std::ostream &os, const point_t &p) {
    return os << '(' << p.x << ", " << p.y << ')';
}
