/**
 * @file constants.hpp
 * @author the-hyp0cr1t3
 * @brief Constants and type settings
 * @date 2022-03-25
 */
#ifndef __CONSTANTS_HPP__
#define __CONSTANTS_HPP__

#ifndef LOGFILE_PATH
#define LOGFILE_PATH "../output/logfile.txt"
#endif

#include <limits>

/// Underlying type for all computations
using float_t = double;

/// Permissible error
inline constexpr float_t EPS = std::numeric_limits<float>::epsilon();
inline constexpr float_t EPS_INC = 5 * EPS;

#endif // __CONSTANTS_HPP__