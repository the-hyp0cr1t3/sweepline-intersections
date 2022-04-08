/**
 * @file point.hpp
 * @author the-hyp0cr1t3
 * @brief Describes the Point struct
 * @date 2022-03-25
 */
#pragma once

#include <constants.hpp>

#include <iostream>


namespace geometry {

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
     * All floating point comparisons are done within a neighbourhood of `::EPS`.
     *
     * @param other The other point this is being compared to
     * @return `true` if both the x and y coordinates match with max permissible error `::EPS`
     * @return `false` otherwise
     */
    bool operator == (const point_t &other) const;
  };

} // namespace geometry