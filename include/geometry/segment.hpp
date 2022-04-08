/**
 * @file segment.hpp
 * @author the-hyp0cr1t3
 * @brief Describes the Segment struct
 * @date 2022-03-25
 */
#pragma once

#include <point.hpp>


namespace geometry {

  /**
   * @brief Segment struct which encapsulates information about a segment
   * @warning \f$ p \le q \f$ must hold for the algorithm to work (comparing on the tuple `<x, y>`)
   */
  struct segment_t {
    /// The point where the segment begins
    point_t p;

    /// The point where the segment ends
    point_t q;

    /// The index of the segment in input
    size_t seg_id;

    /**
     * @brief Evaluates the y coordinate of a point on the segment given its x coordinate
     *
     * @param x The x coordinate of the point to be found on the segment
     * @return `y` The corresponding y coordinate of the desired point
     */
    float_t eval_y(float_t x) const;

    /**
     * @brief  Overloading the < operator overload for segment_t
     *
     * Required by (`std::less<T>`, the defalt comparator of) the segment ordering BBST
     * to make comparisons between events and establish an ordering.
     *
     * Compares y coordinates of segments by calling `segment_t::eval_y()` with the current x coordinate of the sweepline. <br>
     * All floating point comparisons are done within a neighbourhood of `geometry::EPS`.
     *
     * @param s The other segment to compare to
     * @return `true` if the y coordinate corresponding to `sweepline::sweeplineX` is lesser than the other segment's
     * @return `false` otherwise
     */
    bool operator < (const segment_t &s) const;
  };

  /**
   * @brief Checks if two segments intersect in one dimension
   *
   * @param l1 `p.x` (or `p.y`) of the first segment
   * @param r1 `q.x` (or `q.y`) of the first segment
   * @param l2 `p.x` (or `p.y`) of the second segment
   * @param r2 `q.x` (or `q.y`) of the second segment
   * @return `true` if the segments intersect in one dimension
   * @return `false` otherwise
   */
  bool can_intersect_1d(float_t l1, float_t r1, float_t l2, float_t r2);

  /**
   * @brief Computes the sign of the cross product of three points
   *
   * @param a The first point
   * @param b The second point
   * @param c The third point
   * @return `-1` if the cross product is negative
   * @return `0`  if the cross product is zero
   * @return `+1` if the cross product is positive
   */
  int cross_prod(const point_t &a, const point_t &b, const point_t &c);

  /**
   * @brief Checks if two segments intersect
   *
   * Necessary and sufficient conditions for intersection:
   * 1. Segments must intersect one dimensionally on both x and y components.
   * 2. Cross product of the end points of each segment with each end point of
   * the other segment must not have the same sign.
   *
   * @param a The first segment
   * @param b The second segment
   * @return `true` if the segments intersect
   * @return `false` otherwise
   */
  bool is_intersecting(const segment_t &a, const segment_t &b);

  /**
   * @brief Computes the point of intersection of two segments
   * @pre The segments must intersect.
   *
   * @param a The first segment
   * @param b The second segment
   * @return The point of intersection of the two segments
   */
  point_t intersection_point(const segment_t &a, const segment_t &b);

} // namespace geometry
