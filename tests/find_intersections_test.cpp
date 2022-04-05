#include <gtest/gtest.h>
#include <sweepline/sweepline.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

namespace {

using Sweepline::Geometry::EPS;
using Sweepline::Geometry::point_t;
using Sweepline::Geometry::segment_t;
using Sweepline::intersection_t;

class EdgeCases : public testing::Test {

protected:

    std::vector<segment_t> input(const std::string &fname) {
        std::ifstream fin(fname);

        size_t n;       // number of input segments
        fin >> n;

        std::vector<segment_t> segments;
        segments.reserve(n);

        for(size_t i = 0; i < n; i++) {
            Sweepline::Geometry::float_t x1, y1, x2, y2;
            fin >> x1 >> y1 >> x2 >> y2;

            if(std::make_pair(x1, y1) > std::make_pair(x2, y2))
                std::swap(x1, x2), std::swap(y1, y2);

            segments.emplace_back(segment_t{ point_t{ x1, y1 }, point_t{ x2, y2 }, i });
        }

        return segments;
    }

    std::vector<intersection_t> expected_output(const std::string &fname) {
        std::ifstream fin(fname);

        size_t m;       // number of intersection points
        fin >> m;

        std::vector<intersection_t> result;
        result.reserve(m);

        for(size_t i = 0; i < m; i++) {
            Sweepline::Geometry::float_t x, y;
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
                intersection_t {
                    point_t{ x, y },
                    segments
                }
            );
        }

        // sort intersection points by point
        std::sort(result.begin(), result.end(),
            [](const intersection_t &lhs, const intersection_t &rhs) {
                return std::fabs(lhs.pt.x - rhs.pt.x) < EPS?
                        lhs.pt.y < rhs.pt.y - EPS : lhs.pt.x < rhs.pt.x - EPS;
            }
        );

        return result;
    }

    std::vector<intersection_t> normalize(std::vector<intersection_t> &&received) {
        // sort received intersection points by point
        std::sort(received.begin(), received.end(),
            [](const intersection_t &lhs, const intersection_t &rhs) {
                return std::fabs(lhs.pt.x - rhs.pt.x) < EPS?
                        lhs.pt.y < rhs.pt.y - EPS : lhs.pt.x < rhs.pt.x - EPS;
            }
        );

        // sort segments within each intersection and convert segment ids 0-based -> 1-based
        std::for_each(received.begin(), received.end(), [](intersection_t &it) {
            std::sort(it.segments.begin(), it.segments.end());
            std::for_each(it.segments.begin(), it.segments.end(), [](size_t &idx) { ++idx; });
        });

        return received;
    }

};

TEST_F(EdgeCases, AnotherNestedY){
    auto segments = input("edge_case_another_nested_y.txt");
    auto expected = expected_output("expected/edge_case_another_nested_y.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        // point_t operator == is overloaded to work within EPS neighbourhood
        EXPECT_EQ(expected[i].pt, received[i].pt);
        // segment ids are uint, so no precision errors to worry about
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, GridLinesWithSingleOblique){
    auto segments = input("edge_case_grid_lines_with_single_oblique.txt");
    auto expected = expected_output("expected/edge_case_grid_lines_with_single_oblique.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, OriginIntersect3){
    auto segments = input("edge_case_origin_intersect_3.txt");
    auto expected = expected_output("expected/edge_case_origin_intersect_3.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, Butterfly){
    auto segments = input("edge_case_butterfly.txt");
    auto expected = expected_output("expected/edge_case_butterfly.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, HorizontalObliqueCross){
    auto segments = input("edge_case_horizontal_oblique_cross.txt");
    auto expected = expected_output("expected/edge_case_horizontal_oblique_cross.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, ParallelsIntersectOblique){
    auto segments = input("edge_case_parallels_intersect_oblique.txt");
    auto expected = expected_output("expected/edge_case_parallels_intersect_oblique.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, CloseParallelLines){
    auto segments = input("edge_case_close_parallel_lines.txt");
    auto expected = expected_output("expected/edge_case_close_parallel_lines.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, HorizontalParallel){
    auto segments = input("edge_case_horizontal_parallel.txt");
    auto expected = expected_output("expected/edge_case_horizontal_parallel.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, Star){
    auto segments = input("edge_case_star.txt");
    auto expected = expected_output("expected/edge_case_star.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, CoordinateAxes1){
    auto segments = input("edge_case_coordinate_axes_1.txt");
    auto expected = expected_output("expected/edge_case_coordinate_axes_1.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, NarrowingDownwards){
    auto segments = input("edge_case_narrowing_downwards.txt");
    auto expected = expected_output("expected/edge_case_narrowing_downwards.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, TriangleInTriangle){
    auto segments = input("edge_case_triangle_in_triangle.txt");
    auto expected = expected_output("expected/edge_case_triangle_in_triangle.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, CoordinateAxes2){
    auto segments = input("edge_case_coordinate_axes_2.txt");
    auto expected = expected_output("expected/edge_case_coordinate_axes_2.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, NestedY){
    auto segments = input("edge_case_nested_y.txt");
    auto expected = expected_output("expected/edge_case_nested_y.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, VerticalObliqueCross){
    auto segments = input("edge_case_vertical_oblique_cross.txt");
    auto expected = expected_output("expected/edge_case_vertical_oblique_cross.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, CoordinateAxes3){
    auto segments = input("edge_case_coordinate_axes_3.txt");
    auto expected = expected_output("expected/edge_case_coordinate_axes_3.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, NotIntersectingButClose){
    auto segments = input("edge_case_not_intersecting_but_close.txt");
    auto expected = expected_output("expected/edge_case_not_intersecting_but_close.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, VerticalParallel){
    auto segments = input("edge_case_vertical_parallel.txt");
    auto expected = expected_output("expected/edge_case_vertical_parallel.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, DisappointedFace){
    auto segments = input("edge_case_disappointed_face.txt");
    auto expected = expected_output("expected/edge_case_disappointed_face.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, OriginIntersect1){
    auto segments = input("edge_case_origin_intersect_1.txt");
    auto expected = expected_output("expected/edge_case_origin_intersect_1.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

TEST_F(EdgeCases, OriginIntersect2){
    auto segments = input("edge_case_origin_intersect_2.txt");
    auto expected = expected_output("expected/edge_case_origin_intersect_2.txt");
    auto received = normalize(Sweepline::find_intersections(segments));

    EXPECT_EQ(received.size(), expected.size());
    for(size_t i = 0; i < expected.size(); i++) {
        EXPECT_EQ(expected[i].pt, received[i].pt);
        EXPECT_EQ(expected[i].segments, received[i].segments);
    }
}

} // namespace