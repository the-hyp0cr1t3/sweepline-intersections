#pragma once
#include <point.hpp>

namespace Sweepline::Geometry {

  struct event_t {
    /// The point where the event occurs
    point_t p;

    /// An enum for the type of event
    enum type {
      begin,    ///< denotes when a segment starts
      interior, ///< denotes an interior point on a segment
      end       ///< denotes when a segment ends
    } tp;

    /// The id of the segment
    size_t seg_id;

    event_t() = default;


    event_t(const point_t &p, type tp, size_t seg_id)
      : p(p), tp(tp), seg_id(seg_id) {}


    bool operator < (const event_t &e) const;

  };

  std::ostream &operator << (std::ostream &os, const event_t &e);

} // namespace Sweepline::Geometry