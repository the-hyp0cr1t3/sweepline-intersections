#include <gtest/gtest.h>
#include <sweepline.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>


namespace {

class EdgeCases : public testing::Test {

protected:

    std::vector<geometry::segment_t> input(const std::string &fname) {
        std::ifstream fin(fname);

        size_t n;       // number of input segments
        fin >> n;

        std::vector<geometry::segment_t> segments;
        segments.reserve(n);

        for(size_t i = 0; i < n; i++) {
            geometry::float_t x1, y1, x2, y2;
            fin >> x1 >> y1 >> x2 >> y2;

            if(std::make_pair(x1, y1) > std::make_pair(x2, y2))
                std::swap(x1, x2), std::swap(y1, y2);

            segments.emplace_back(geometry::segment_t{ geometry::point_t{ x1, y1 }, geometry::point_t{ x2, y2 }, i });
        }

        return segments;
    }

    std::vector<sweepline::intersection_t> expected_output(const std::string &fname) {
        std::ifstream fin(fname);

        size_t m;       // number of intersection points
        fin >> m;

        std::vector<sweepline::intersection_t> result;
        result.reserve(m);

        for(size_t i = 0; i < m; i++) {
            geometry::float_t x, y;
            fin >> x >> y;

            std::vector<size_t> segments;

            size_t idx;
            std::string s;
            std::getline(fin, s);
            std::istringstream iss(s);
            while(iss >> idx)
                segments.push_back(idx);

            // sort the vector of segments by id
            std::sort(segments.begin(), segments.end());

            result.emplace_back(
                sweepline::intersection_t {
                    geometry::point_t{ x, y },
                    segments
                }
            );
        }

        // sort intersection points by point
        std::sort(result.begin(), result.end(),
            [](const sweepline::intersection_t &lhs, const sweepline::intersection_t &rhs) {
                return std::fabs(lhs.pt.x - rhs.pt.x) < geometry::EPS?
                        lhs.pt.y < rhs.pt.y - geometry::EPS : lhs.pt.x < rhs.pt.x - geometry::EPS;
            }
        );

        return result;
    }

    std::vector<sweepline::intersection_t> normalize(std::vector<sweepline::intersection_t> &&received) {
        // sort received intersection points by point
        std::sort(received.begin(), received.end(),
            [](const sweepline::intersection_t &lhs, const sweepline::intersection_t &rhs) {
                return std::fabs(lhs.pt.x - rhs.pt.x) < geometry::EPS?
                        lhs.pt.y < rhs.pt.y - geometry::EPS : lhs.pt.x < rhs.pt.x - geometry::EPS;
            }
        );

        // sort segments within each intersection and convert segment ids 0-based -> 1-based
        std::for_each(received.begin(), received.end(), [](sweepline::intersection_t &it) {
            std::sort(it.segments.begin(), it.segments.end());
            std::for_each(it.segments.begin(), it.segments.end(), [](size_t &idx) { ++idx; });
        });

        return received;
    }

};

#define DO_EDGE_CASE(inputf)                                              \
    auto segments = input(inputf);                                        \
    auto expected = expected_output("expected/" inputf);                  \
    auto received = normalize(sweepline::find_intersections(segments));   \
                                                                          \
    EXPECT_EQ(received.size(), expected.size());                          \
    for (size_t i = 0; i < expected.size(); i++) {                        \
        /* point_t  == is overloaded to work within EPS neighbourhood */  \
        EXPECT_EQ(expected[i].pt, received[i].pt);                        \
        /* segment ids are uint, so no precision errors to worry about */ \
        EXPECT_EQ(expected[i].segments, received[i].segments);            \
    }

TEST_F(EdgeCases, AnotherNestedY){
    DO_EDGE_CASE("edge_case_another_nested_y.txt")
}

TEST_F(EdgeCases, GridLinesWithSingleOblique){
    DO_EDGE_CASE("edge_case_grid_lines_with_single_oblique.txt")
}

TEST_F(EdgeCases, OriginIntersect3){
    DO_EDGE_CASE("edge_case_origin_intersect_3.txt")
}

TEST_F(EdgeCases, Butterfly){
    DO_EDGE_CASE("edge_case_butterfly.txt")
}

TEST_F(EdgeCases, HorizontalObliqueCross){
    DO_EDGE_CASE("edge_case_horizontal_oblique_cross.txt")
}

TEST_F(EdgeCases, ParallelsIntersectOblique){
    DO_EDGE_CASE("edge_case_parallels_intersect_oblique.txt")
}

TEST_F(EdgeCases, CloseParallelLines){
    DO_EDGE_CASE("edge_case_close_parallel_lines.txt")
}

TEST_F(EdgeCases, HorizontalParallel){
    DO_EDGE_CASE("edge_case_horizontal_parallel.txt")
}

TEST_F(EdgeCases, Star){
    DO_EDGE_CASE("edge_case_star.txt")
}

TEST_F(EdgeCases, CoordinateAxes1){
    DO_EDGE_CASE("edge_case_coordinate_axes_1.txt")
}

TEST_F(EdgeCases, NarrowingDownwards){
    DO_EDGE_CASE("edge_case_narrowing_downwards.txt")
}

TEST_F(EdgeCases, TriangleInTriangle){
    DO_EDGE_CASE("edge_case_triangle_in_triangle.txt")
}

TEST_F(EdgeCases, CoordinateAxes2){
    DO_EDGE_CASE("edge_case_coordinate_axes_2.txt")
}

TEST_F(EdgeCases, NestedY){
    DO_EDGE_CASE("edge_case_nested_y.txt")
}

TEST_F(EdgeCases, VerticalObliqueCross){
    DO_EDGE_CASE("edge_case_vertical_oblique_cross.txt")
}

TEST_F(EdgeCases, CoordinateAxes3){
    DO_EDGE_CASE("edge_case_coordinate_axes_3.txt")
}

TEST_F(EdgeCases, NotIntersectingButClose){
    DO_EDGE_CASE("edge_case_not_intersecting_but_close.txt")
}

TEST_F(EdgeCases, VerticalParallel){
    DO_EDGE_CASE("edge_case_vertical_parallel.txt")
}

TEST_F(EdgeCases, DisappointedFace){
    DO_EDGE_CASE("edge_case_disappointed_face.txt")
}

TEST_F(EdgeCases, OriginIntersect1){
    DO_EDGE_CASE("edge_case_origin_intersect_1.txt")
}

TEST_F(EdgeCases, OriginIntersect2){
    DO_EDGE_CASE("edge_case_origin_intersect_2.txt")
}

} // namespace