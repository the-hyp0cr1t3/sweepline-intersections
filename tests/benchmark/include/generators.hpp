#pragma once

#include <segment.hpp>
#include <vector>

namespace generators {

std::vector<geometry::segment_t> gen_oblique_grid(size_t num_horiz, size_t num_verti);

std::vector<geometry::segment_t> gen_origin_star(size_t num_segments);

} // namespace generators
