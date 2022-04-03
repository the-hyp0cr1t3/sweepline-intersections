/**
 * @file intersection.hpp
 * @authors the-hyp0cr1t3 JeevanJyot
 * @brief Main logic to find intersections
 * @date 2022-03-25
 */
#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include <vector>
#include <utility>
#include <point.hpp>
#include <event.hpp>
#include <segment.hpp>
#include <intersection.hpp>
#include <red_black_tree.tpp>

/**
 * @brief Simple struct to bind together information on an intersection of two or more segments
 */
struct intersection_t {
    /// The point of intersection of some segments
    point_t pt;

    /// A list of segments (their ids) which intersect at this point
    std::vector<size_t> segments;
};

/**
 * @brief Finds which segments intersect at which points and returns all such intersections
 * @pre p <= q must hold for each line segment in the list
 * @pre No two line segments should coincide with each other fully or partially
 *
 * Takes a list of line segments whose end points are assumed to be in sorted order (left to right).
 * Uses the Bentley Ottman algorithm to compute all line segment intersections efficiently in O((n+k)logn),
 * where n is the number of line segments and k is the number of intersection points.
 *
 * Returns a list of intersections, i.e. pairs of points and the corresponding
 * indices (1-based) of segments which intersect at that point.
 *
 * @param line_segments The list of segments
 * @return std::vector<intersection_t> A list of all intersections
 */
auto find_intersections(const std::vector<segment_t> &line_segments)
    -> std::vector<intersection_t>;

/**
 * @brief << operator overload for intersection_t
 *
 * @param os The output stream to write to
 * @param s The intersection to be written
 * @return std::ostream& A reference to the output stream after writing to it
 */
std::ostream &operator << (std::ostream &os, const intersection_t &s);


#ifndef NDEBUG
    template <class T>
    std::ostream &operator << (std::ostream &os, const std::vector<T> &v) {
        os << '{';
        bool fst = true;
        for(auto &x: v)
            os << (fst? "" : ", ") << x, fst = false;
        return os << '}';
    }
#endif

#endif // __INTERSECTION_HPP__