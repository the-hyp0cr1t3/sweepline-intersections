#pragma once

#include <iostream>
#include <sweepline/geometry/constants.hpp>

namespace Sweepline::Geometry {

  /**
   * @brief Simple point struct to store the x and y coordinates of point entities
   */
  struct point_t {
    /// The x coordinate of the point
    float_t x;

    /// The y coordinate of the point
    float_t y;

    /**
     * @brief == operator overload for point_t
     *
     * @param other The other point this is being compared to
     * @return true if the x and y coordinates match with error < EPS
     * @return false otherwise
     */
    bool operator == (const point_t &other) const;
  };

  /**
   * @brief << operator overload for point_t
   *
   * @param os The output stream to write to
   * @param p The point to be written
   * @return std::ostream& A reference to the output stream after writing to it
   */
  std::ostream &operator << (std::ostream &os, const point_t &p);

} // namespace Sweepline::Geometry