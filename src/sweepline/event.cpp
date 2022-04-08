#include <event.hpp>
#include <cmath>

bool sweepline::event_t::operator < (const event_t &e) const {
    if(std::fabs(p.x - e.p.x) > geometry::EPS)
        return p.x < e.p.x;
    else if(std::fabs(p.y - e.p.y) > geometry::EPS)
        return p.y < e.p.y;
    else
        return seg_id < e.seg_id;
}