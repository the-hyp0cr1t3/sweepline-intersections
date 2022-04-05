#pragma once

#include <vector>
#include <utility>
#include <sweepline/geometry/point.hpp>
#include <sweepline/geometry/event.hpp>
#include <sweepline/geometry/segment.hpp>

namespace Sweepline {

  /**
   * @brief Simple struct to bind together information on an intersection of two or more segments
   */
  struct intersection_t {
    /// The point of intersection of some segments
    Geometry::point_t pt;

    /// A list of segments (their ids) which intersect at this point
    std::vector<size_t> segments;
  };

  /**
   * @brief Finds which segments intersect at which points and returns all such intersections
   * @pre p <= q must hold for each line segment in the list
   * @pre No two line segments should coincide with each other fully or partially
   *
   * Takes a list of line segments whose end points are assumed to be in sorted order (left to right).
   * Uses the Bentley Ottman algorithm to compute all line segment intersections efficiently in O((n+k)logn),
   * where n is the number of line segments and k is the number of intersection points.
   *
   * Returns a list of intersections, i.e. pairs of points and the corresponding
   * indices (1-based) of segments which intersect at that point.
   *
   * @param line_segments The list of segments
   * @return std::vector<intersection_t> A list of all intersections
   */
  auto find_intersections(const std::vector<Geometry::segment_t> &line_segments, bool verbose = false, bool enable_color = true)
    -> std::vector<intersection_t>;

} // namespace Sweepline
