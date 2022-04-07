#pragma once
#include <vector>
#include <sweepline/geometry/segment.hpp>

using Sweepline::Geometry::segment_t;
using Sweepline::Geometry::point_t;
using _float_t = Sweepline::Geometry::float_t;

std::vector<segment_t> gen_oblique_grid(size_t num_horiz, size_t num_verti);

std::vector<segment_t> gen_origin_star(size_t num_segments);