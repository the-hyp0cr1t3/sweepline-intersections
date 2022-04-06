/**
 * @file constants.hpp
 * @author the-hyp0cr1t3
 * @brief Constants and type aliases
 * @date 2022-03-25
 */
#pragma once
#include <limits>

namespace Sweepline::Geometry {

  /// Underlying type for all computations
  using float_t = double;

  /// Permissible error; in other words, limiting accuracy
  inline constexpr float_t EPS = std::numeric_limits<float>::epsilon();

  inline constexpr float_t EPS_INC = 5 * EPS;

} // namespace Sweepline::Geometry