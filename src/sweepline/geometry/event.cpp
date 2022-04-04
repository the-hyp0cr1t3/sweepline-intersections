#include <event.hpp>

#include <cmath>

namespace Sweepline::Geometry {

bool event_t::operator < (const event_t &e) const {
    if(std::fabs(p.x - e.p.x) > EPS)
        return p.x < e.p.x;
    else if(std::fabs(p.y - e.p.y) > EPS)
        return p.y < e.p.y;
    else
        return seg_id < e.seg_id;
}

#ifndef NDEBUG
    std::ostream &operator << (std::ostream &os, const event_t &e) {
        return os << "<(" << e.p.x << ", " << e.p.y << "), "
                << (e.tp == event_t::type::begin? "begin"
                    : e.tp == event_t::type::interior? "interior" : "end") << ", id=" << e.seg_id + 1 << '>';
    }
#endif

} // namespace Sweepline::Geometry