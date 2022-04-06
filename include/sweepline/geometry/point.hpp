/**
 * @file point.hpp
 * @author the-hyp0cr1t3
 * @brief Describes the point_t struct
 * @date 2022-03-25
 */
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
     * @brief Overloading the == operator for point_t
     *
     * All floating point comparisons are done within a neighbourhood of `Sweepline::Geometry::EPS`.
     *
     * @param other The other point this is being compared to
     * @return `true` if both the x and y coordinates match with max permissible error EPS
     * @return `false` otherwise
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
  // std::ostream &operator << (std::ostream &os, const point_t &p);

} // namespace Sweepline::Geometry