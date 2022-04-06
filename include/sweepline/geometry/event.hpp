/**
 * @file event.hpp
 * @author the-hyp0cr1t3
 * @brief Describes the event_t struct
 * @date 2022-03-25
 */
#pragma once
#include <sweepline/geometry/point.hpp>

namespace Sweepline::Geometry {

  /**
   * @brief Event struct which encapsulates information about an event
   *
   * @note Multiple events having the same event point may exist simultaneously
   * within the event queue, one for each segment containing its `seg_id`.
   *
   */
  struct event_t {
    /// The point where the event occurs
    point_t p;

    /// An enum for the type of event
    enum type {
      begin,    ///< denotes when a segment starts
      interior, ///< denotes an interior point of a segment
      end       ///< denotes when a segment ends
    } tp;   ///< The type of the event

    /// The id of the segment
    size_t seg_id;

    /**
     * @brief Default constructor
     */
    event_t() = default;

    /**
     * @brief Parameterized constructor
     *
     * @param p The point where the event occurs
     * @param tp The type of the event
     * @param seg_id The id of the segment
     */
    event_t(const point_t &p, type tp, size_t seg_id)
      : p(p), tp(tp), seg_id(seg_id) {}

    /**
     * @brief Overloading the < operator for event_t
     *
     * Required by (`std::less<T>`, the defalt comparator of) the event queue BBST
     * to make comparisons between events and establish an ordering.
     *
     * Compares on, in decreasing priority, the tuple `<p.x, p.y, seg_id>`.
     * All floating point comparisons are done within a neighbourhood of `Sweepline::Geometry::EPS`.
     *
     * @param e The other event to be compared to
     * @return `true` if it compares less than the other event
     * @return `false` otherwise
     */
    bool operator < (const event_t &e) const;

  };

} // namespace Sweepline::Geometry