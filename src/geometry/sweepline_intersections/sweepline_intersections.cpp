#include <iostream>
#include <array>
#include <vector>
#include <limits>
#include <iomanip>
#include <intersection.hpp>

float_t sweepline_X;

// #include <set>
// template<typename T>
// using bbst = std::set<T>;

template<typename T>
using bbst = red_black_tree<T>;

std::vector<intersection_t> find_intersections(const std::vector<segment_t> &line_segments) {
    std::vector<intersection_t> result;
    sweepline_X = -std::numeric_limits<float_t>::max();

#ifndef NDEBUG
    std::ofstream ferr(LOGFILE_PATH, std::ios::trunc);

    ferr << "line_segments = {" << std::endl;
    for(auto &x: line_segments)
        ferr << '\t' << x << std::endl;
    ferr << '}' << std::endl << std::endl;
#endif

    bbst<event_t> event_queue;
    bbst<segment_t> seg_ordering;
    std::vector<segment_t> vertical_segs;

    size_t n = line_segments.size();
    for(size_t i = 0; i < n; i++) {
        const point_t &p = line_segments[i].p;
        const point_t &q = line_segments[i].q;

        if(abss(p.x - q.x) < EPS) {
            vertical_segs.emplace_back(line_segments[i]);
        } else {
            event_queue.insert({ p, event_t::type::begin, i});
            event_queue.insert({ q, event_t::type::end,   i});
        }
    }

#ifndef NDEBUG
    ferr << "vertical_segs = " << vertical_segs << std::endl;
    ferr << std::endl << "------------------------------------" << std::endl;
#endif

    std::sort(vertical_segs.begin(), vertical_segs.end(),
        [](const segment_t &a, const segment_t &b) {
            return a.p.x == b.p.x? a.p.y < b.p.y : a.p.x < b.p.x;
        }
    );

    for(size_t i = 0; i + 1 < vertical_segs.size(); i++) {
        if(abss(vertical_segs[i].q.y - vertical_segs[i + 1].p.y) < EPS) {
            intersection_t it { vertical_segs[i].q,
                                std::vector<size_t>{ vertical_segs[i].seg_id, vertical_segs[i + 1].seg_id } };
#ifndef NDEBUG
            ferr << std::endl << "Found vertical-vertical segment intersection!" << std::endl;
            ferr << it << std::endl;
#endif
            result.emplace_back(it);
        }
    }

    size_t vert_idx = 0;

    while(!event_queue.empty()) {
        event_t top = *event_queue.begin();
        event_queue.erase(event_queue.begin());

        if(top.p.x < sweepline_X) {
#ifndef NDEBUG
            ferr << "top.p.x = " << top.p.x << std::endl;
            ferr << "sweepline_X = " << sweepline_X << std::endl;
            ferr << "continuing..." << std::endl;
#endif
            continue;
        }

        while(vert_idx < vertical_segs.size()
            and vertical_segs[vert_idx].p.x < sweepline_X - EPS) {
                vert_idx++;
        }

        while(vert_idx < vertical_segs.size()
            and vertical_segs[vert_idx].p.x <= top.p.x + EPS) {
                auto &vseg = vertical_segs[vert_idx];
                sweepline_X = vseg.p.x;
                auto it = seg_ordering.lower_bound(segment_t{ vseg.p, vseg.p, 0 });
                while(it != seg_ordering.end()) {
                    float_t it_y = it->eval_y(sweepline_X);
                    if(it_y > vseg.q.y + EPS) break;
                    intersection_t itrsctn { point_t{ sweepline_X, it_y },
                                        std::vector<size_t>{ it->seg_id, vseg.seg_id } };
#ifndef NDEBUG
                    ferr << std::endl << "Found vertical-non vertical segment intersection!" << std::endl;
                    ferr << itrsctn << std::endl;
#endif
                    result.emplace_back(itrsctn);
                    ++it;
                }

                vert_idx++;
        }

        sweepline_X = top.p.x;

#ifndef NDEBUG
        ferr << std::endl << "sweepline_X = " << sweepline_X << std::endl << std::endl;
        ferr << "initially:" << std::endl;
        ferr << "event_queue = {";
        ferr << top;
        for(auto &x: event_queue)
            ferr << ", " << x;
        ferr << '}' << std::endl;

        ferr << "segment_ordering = {";
        {
            bool fst = true;
            for(auto &x: seg_ordering)
                ferr << (fst? "" : ", ") << x.seg_id + 1, fst = false;
            ferr << '}' << std::endl;
        }
        ferr << std::endl;
#endif

        std::array<std::vector<size_t>, 3> active;
        active[top.tp].push_back(top.seg_id);

        while(!event_queue.empty()
                and abss(event_queue.begin()->p.x - top.p.x) < EPS
                and abss(event_queue.begin()->p.y - top.p.y) < EPS) {

            event_t nxt_top = *event_queue.begin();
            event_queue.erase(event_queue.begin());
            active[nxt_top.tp].push_back(nxt_top.seg_id);
        }

#ifndef NDEBUG
        ferr << "active point -> " << top.p << std::endl;
        ferr << "segments with end point here      = ";
        ferr << active[event_t::type::end] << std::endl;

        ferr << "segments with interior point here = ";
        ferr << active[event_t::type::interior] << std::endl;

        ferr << "segments with begin point here    = ";
        ferr << active[event_t::type::begin] << std::endl;
#endif

        for(int idx: active[event_t::type::end])
            seg_ordering.erase(line_segments[idx]);

        for(int idx: active[event_t::type::interior])
            seg_ordering.erase(line_segments[idx]);

        sweepline_X += EPS_INC;
        float_t mx = -std::numeric_limits<float_t>::max();
        float_t mn = std::numeric_limits<float_t>::max();

        for(int idx: active[event_t::type::begin]) {
            mn = std::min(mn, line_segments[idx].eval_y(sweepline_X));
            mx = std::max(mx, line_segments[idx].eval_y(sweepline_X));
            seg_ordering.insert(line_segments[idx]);
        }

        for(int idx: active[event_t::type::interior]) {
            mn = std::min(mn, line_segments[idx].eval_y(sweepline_X));
            mx = std::max(mx, line_segments[idx].eval_y(sweepline_X));
            seg_ordering.insert(line_segments[idx]);
        }

        if(active[event_t::type::begin].size() + active[event_t::type::interior].size() == 0) {
            auto b_right = seg_ordering.lower_bound(segment_t{ top.p, top.p, 0 });
            if(b_right != seg_ordering.end() and b_right != seg_ordering.begin()) {
                auto b_left = b_right;
                --b_left;
                if(is_intersecting(*b_left, *b_right)) {
                    point_t pt = intersection_point(*b_left, *b_right);
                    event_t::type tp1 = b_left->p == pt? event_t::type::begin : event_t::type::interior;
                    event_t::type tp2 = b_right->p == pt? event_t::type::begin : event_t::type::interior;

                    event_queue.insert(event_t{ pt, tp1, b_left->seg_id  });
                    event_queue.insert(event_t{ pt, tp2, b_right->seg_id });
                }
            }
        } else {
            point_t left { sweepline_X, mn - 2 * EPS }, right { sweepline_X, mx + 2 * EPS };
            auto b_right = seg_ordering.lower_bound(segment_t{ right, right, 0 });
            auto s_left  = seg_ordering.lower_bound(segment_t{ left,  left,  0 });

            if(b_right != seg_ordering.end() and b_right != seg_ordering.begin()) {
                auto s_right = b_right;
                --s_right;

                if(is_intersecting(*s_right, *b_right)) {
                    point_t pt = intersection_point(*s_right, *b_right);
                    event_t::type tp1 = s_right->p == pt? event_t::type::begin : event_t::type::interior;
                    event_t::type tp2 = b_right->p == pt? event_t::type::begin : event_t::type::interior;

                    event_queue.insert(event_t{ pt, tp1, s_right->seg_id });
                    event_queue.insert(event_t{ pt, tp2, b_right->seg_id });
                }
            }

            if(s_left != seg_ordering.end() and s_left != seg_ordering.begin()) {
                auto b_left = s_left;
                --b_left;

                if(is_intersecting(*b_left, *s_left)) {
                    point_t pt = intersection_point(*b_left, *s_left);
                    event_t::type tp1 = b_left->p == pt? event_t::type::begin : event_t::type::interior;
                    event_t::type tp2 = s_left->p == pt? event_t::type::begin : event_t::type::interior;

                    event_queue.insert(event_t{ pt, tp1, b_left->seg_id });
                    event_queue.insert(event_t{ pt, tp2, s_left->seg_id });
                }
            }
        }

        sweepline_X -= EPS_INC;

        // we have an intersection
        if(active[0].size() + active[1].size() + active[2].size() > 1) {
            active[0].insert(active[0].end(), active[1].begin(), active[1].end());
            active[0].insert(active[0].end(), active[2].begin(), active[2].end());

            result.emplace_back(intersection_t{ top.p, active[0] });

#ifndef NDEBUG
            ferr << std::endl << "Found intersection!" << std::endl;
            ferr << result.back() << std::endl;
#endif

        }

#ifndef NDEBUG
        ferr << std::endl << "finally:" << std::endl;

        ferr << "event_queue = {";
        {
            bool fst = true;
            for(auto &x: event_queue)
                ferr << (fst? "" : ", ") << x, fst = false;
            ferr << '}' << std::endl;
        }

        ferr << "segment_ordering = {";
        {
            bool fst = true;
            for(auto &x: seg_ordering)
                ferr << (fst? "" : ", ") << x.seg_id + 1, fst = false;
            ferr << '}' << std::endl;
        }

        ferr << std::endl << "------------------------------------" << std::endl;
#endif
    }

    std::sort(result.begin(), result.end(),
        [](const intersection_t &a, const intersection_t &b) {
            return a.pt.x == b.pt.x? a.pt.y < b.pt.y : a.pt.x < b.pt.x;
        }
    );

    std::vector<intersection_t> answer;
    for(size_t i = 0, j; i < result.size(); i = j) {
        j = i;
        std::vector<int> indices;
        while(j < result.size() and result[j].pt == result[i].pt) {
            indices.insert(indices.end(), result[j].segments.begin(), result[j].segments.end());
            j++;
        }

        std::sort(indices.begin(), indices.end());
        indices.erase(std::unique(indices.begin(), indices.end()), indices.end());

        answer.emplace_back(intersection_t{
                                result[i].pt,
                                std::vector<size_t>(indices.begin(), indices.end())
                            });
    }

    return answer;
}

std::ostream &operator << (std::ostream &os, const intersection_t &s) {
    std::ostringstream tmp;
    tmp.precision(5);
    tmp << '@' << s.pt << ": ";
    os.flags(std::ios::left);
    os.width(9 + 10);
    os << tmp.str() << "{ ";
    bool fst = true;
    for(int idx: s.segments)
        os << (fst? "" : ", ") << idx + 1, fst = false;
    return os << " }";
}