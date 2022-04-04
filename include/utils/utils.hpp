/**
 * @file utils.hpp
 * @author the-hyp0cr1t3
 * @brief Utility functions
 * @date 2022-03-25
 */
#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#define EXAMPLES_PATH       "data/"

#include <fstream>
#include <iostream>

namespace utils {

/**
 * @brief Safely opens a file, otherwise throws an exception
 *
 * @param inFile A reference to an ifstream object to open the file with
 * @param fname The path to the file
 * @note fname must either be an absolute path or a path relative to examples/ dir
 */
void open_file(std::ifstream &inFile, const std::string &fname);

} // namespace utils

#endif // _UTILS_HPP_