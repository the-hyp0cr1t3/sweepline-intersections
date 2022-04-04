#pragma once

#include <limits>

namespace Sweepline::Geometry {

using float_t = double;

inline constexpr float_t EPS = std::numeric_limits<float>::epsilon();

inline constexpr float_t EPS_INC = 5 * EPS;

} // namespace Sweepline::Geometry