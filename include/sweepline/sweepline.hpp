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
   * Calls `solver::solve()` and returns the result.
   *
   * @param line_segments The list of input line segments
   * @param verbose The `utils::args::verbose` flag
   * @param enable_color The `utils::args::enable_color` flag
   * @return `std::vector<intersection_t>` A list of all intersections
   */
  std::vector<intersection_t> find_intersections(
    const std::vector<geometry::segment_t> &line_segments,
    bool verbose = false,
    bool enable_color = true
  );

  /**
   * @brief A utility class instantiated by `find_intersections()`
   * to manage the data structures and implement the algorithm
   * to find all intersection points.
   *
   */
  class solver {
    bool verbose;                                     ///< The `utils::args::verbose` flag
    std::vector<geometry::segment_t> line_segments;   ///< The list of input line segments
    std::vector<sweepline::intersection_t> result;    ///< The list of intersections that will be returned

    bbst<sweepline::event_t> event_queue;             ///< The event queue, implemented as a BBST of events
    bbst<geometry::segment_t> seg_ordering;           ///< The status queue, or segment ordering, implemented as a BBST of segments
    std::vector<geometry::segment_t> vertical_segs;   ///< A list of line segments with slope parallel to the sweepline (vertical) that will be handled separately

  public:

    /**
     * @brief Constructor
     *
     * @param line_segments The list of input line segments
     * @param verbose The `utils::args::verbose` flag
     * @param enable_color The `utils::args::enable_color` flag
     */
    solver(const std::vector<geometry::segment_t> &line_segments, bool verbose, bool enable_color);

    /**
     * @brief Finds which segments intersect at which points and returns all such intersections
     *
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
     * @return `std::vector<intersection_t>` A list of all intersections
     */
    std::vector<sweepline::intersection_t> solve();

  private:
  // Implementation

    /**
     * @brief Initializes the `solver::event_queue` by inserting the end points of the `solver::line_segments` and populates `solver::vertical_segs` with vertical segments
     * @pre \f$ p \le q \f$ must hold for each line segment in the list.
     *
     * Iterates over the segments in `solver::line_segments`
     * 1. Vertical segments are added to `solver::vertical_segs`
     * 2. For all other segments, the begin and end points are inserted into the `solver::event_queue` as begin and end events respectively
     *
     */
    void init_event_queue();

    /**
     * @brief Finds intersections between pairs of vertical line segments
     *
     * There can be at most two non-coinciding and intersecting vertical line segments.
     * Such intersections are found by checking if adjacent pairs of segments in sorted order (by x and then y) intersect.
     *
     */
    void find_vertical_vertical_intersections();

    /**
     * @brief Finds intersections between a vertical line segment and one or more non-vertical segments
     *
     * Iterates over vertical line segments (ordered by x) whose `x` coordinate lies before the new sweepline.
     * Non-vertical segments which intersect with a particular vertical segment are found by querying `solver::seg_ordering`.
     *
     * @param max_vsegx The `x` coordinate of the next event, i.e. the new sweepline.
     */
    void find_vertical_nonvertical_intersections(geometry::float_t max_vsegx);

    /**
     * @brief Gets the active segments with an event at the point currently being processed
     *
     * @param top One of the events at the point currently being processed
     * @return `std::array<std::vector<size_t>, 3>` Three arrays of active segment indices corresponding to `event_t::type`
     */
    std::array<std::vector<size_t>, 3> get_active_segs(sweepline::event_t top);

    /**
     * @brief Updates the status queue `solver::seg_ordering` after processing the event point
     *
     * * Removes all segments with `event_t::type::end` and `event_t::type::interior` events
     * * Increments the sweepline by a very small amount, just past the intersection point
     * * Inserts all segments with `event_t::type::begin` events and reinserts the removed
     * segments with `event_t::type::interior` events in the correct order
     *
     * @param active_segs The active segments with an event at the point currently being processed
     */
    void update_segment_ordering(const std::array<std::vector<size_t>, 3> &active_segs);

    void handle_no_newly_inserted(geometry::point_t cur);
    void handle_extremes_of_newly_inserted();
    void report_intersection(geometry::point_t cur, std::array<std::vector<size_t>, 3> &&active_segs);
    void merge_intersection_points();

    /// \cond
    size_t vert_idx = 0;
    geometry::float_t max_y, min_y;
    /// \endcond
  };

} // namespace sweepline
