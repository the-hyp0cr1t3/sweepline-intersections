/**
 * @file sweepline.hpp
 * @authors the-hyp0cr1t3 JeevanJyot
 * @brief Main logic to find intersections
 * @date 2022-03-25
 */
#pragma once

#include <red_black_tree.tpp>
#include <point.hpp>
#include <segment.hpp>
#include <event.hpp>

#include <vector>
#include <array>
#include <utility>
#include <set>

namespace sweepline {

  /// A global variable which stores the current X coordinate of the vertical sweepline
  extern geometry::float_t sweeplineX;

  /**
   * @brief Simple struct to bind together information on an intersection of two or more segments
   */
  struct intersection_t {
    geometry::point_t pt;   ///< The point of intersection of some segments
    std::vector<size_t> segments;   ///< A list of segments (their ids) which intersect at this point
  };

  template <typename T>
  using bbst = BBST::red_black_tree<T>;
  // using bbst = std::set<T>; // works with std::set in exactly the same way (don't forget to #include <set>)

  class solver {
    bool verbose;
    std::vector<geometry::segment_t> line_segments;
    std::vector<sweepline::intersection_t> result;

    bbst<sweepline::event_t> event_queue;
    bbst<geometry::segment_t> seg_ordering;
    std::vector<geometry::segment_t> vertical_segs;

    // impl
    size_t vert_idx = 0;
    geometry::float_t max_y, min_y;

  public:
    solver(const std::vector<geometry::segment_t> &line_segments, bool verbose, bool enable_color);

    std::vector<sweepline::intersection_t> solve();

  private:
    void init_event_queue();
    void find_vertical_vertical_intersections();
    void find_vertical_nonvertical_intersections(geometry::float_t max_vsegx);
    std::array<std::vector<size_t>, 3> get_active_segs(sweepline::event_t top);
    void update_segment_ordering(const std::array<std::vector<size_t>, 3> &active_segs);
    void handle_no_newly_inserted(geometry::point_t cur);
    void handle_extremes_of_newly_inserted();
    void report_intersection(geometry::point_t cur, std::array<std::vector<size_t>, 3> &&active_segs);
    void merge_intersection_points();
  };

  /**
   * @brief Finds which segments intersect at which points and returns all such intersections
   * @pre \f$ p \le q \f$ must hold for each line segment in the list.
   * @pre No two line segments should coincide with each other either fully or partially.
   *
   * Takes a list of line segments whose end points are assumed to be in sorted order (left to right).
   *
   * Uses the Bentley Ottman algorithm to compute all line segment intersections efficiently in \f$ \mathcal{O}((n+k)logn) \f$, <br>
   * where \f$ n \f$ is the number of line segments and \f$ k \f$ is the number of intersection points.
   *
   * Returns a list of intersections, i.e. pairs of points and the corresponding
   * indices (1-based) of segments which intersect at that point.
   *
   * @param line_segments The list of segments
   * @param verbose The `Utils::args::verbose` flag
   * @param enable_color The `Utils::args::enable_color` flag
   * @return `std::vector<intersection_t>` A list of all intersections
   */
  auto find_intersections (
    const std::vector<geometry::segment_t> &line_segments,
    bool verbose = false,
    bool enable_color = true
  )
    -> std::vector<intersection_t>;

} // namespace sweepline
