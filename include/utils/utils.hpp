/**
 * @file utils.hpp
 * @author the-hyp0cr1t3
 * @brief Utility functions
 * @date 2022-03-25
 */
#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#define EXAMPLES_PATH       "examples/"

#include <fstream>
#include <iostream>
#include <constants.hpp>

/**
 * @brief Absolute value function for float_t
 *
 * Spent 4 hours ~debugging~ wasting my life on this all because
 * the standard abs from iostream does not take floating points
 * but does not throw any errors when you pass it one either
 * so screw that im using my own function
 * thanks for the trust issues i will always carry with me
 * for the rest of my life :)
 *
 * @param x A value of type float_t whose absolute value is to be found
 * @return The absolute value of x
 */
float_t abss(float_t x);

/**
 * @brief Safely opens a file, otherwise throws an exception
 *
 * @param inFile A reference to an ifstream object to open the file with
 * @param fname The path to the file
 * @note fname must either be an absolute path or a path relative to examples/ dir
 */
void open_file(std::ifstream &inFile, const std::string &fname);

#endif // _UTILS_HPP_