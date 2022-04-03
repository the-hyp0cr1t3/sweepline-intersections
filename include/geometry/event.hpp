/**
 * @file event.hpp
 * @author the-hyp0cr1t3
 * @brief Describes the event_t struct
 * @date 2022-03-25
 */
#ifndef __EVENT_HPP__
#define __EVENT_HPP__

#include <point.hpp>

/**
 * @brief Event struct which encapsulates information on an event
 */
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

    /**
     * @brief Construct a new event_t object
     */
    event_t() = default;

    /**
     * @brief Construct a new event t object
     *
     * @param p The point where the event occurs
     * @param tp The type of the event
     * @param seg_id The id of the segment
     */
    event_t(const point_t &p, type tp, size_t seg_id)
        : p(p), tp(tp), seg_id(seg_id) {}

    /**
     * @brief < operator overload for event_t
     *
     * Required by the bbst to make comparisons between events.
     *
     * Compares on, in decreasing priority, the tuple <p.x, p.y, seg_id>.
     *
     * @param e The other event to be compared to
     * @return true if it compares less than the other event
     * @return false otherwise
     */
    bool operator < (const event_t &e) const;

};

#ifndef NDEBUG
    std::ostream &operator << (std::ostream &os, const event_t &e);
#endif

#endif // __EVENT_HPP__