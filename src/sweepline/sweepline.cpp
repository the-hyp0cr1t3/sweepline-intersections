#include <iostream>
#include <array>
#include <vector>
#include <limits>
#include <iomanip>
#include <cmath>
#include <fmt/format.h>
#include <fmt/color.h>

#include <sweepline.hpp>
#include <rbtree.tpp>

#define format_col(enable_color, ts, argn...) \
  fmt::format((enable_color? ts : fmt::v8::text_style()), argn)

// #include <set>
// template<typename T>
// using bbst = std::set<T>;

template<typename T>
using bbst = red_black_tree<T>;

namespace Sweepline {

  namespace detail {
    std::array<fmt::color, 3> type_col {
      fmt::color::light_sea_green,  // Geometry::event_t::type::begin
      fmt::color::medium_purple,    // Geometry::event_t::type::interior
      fmt::color::light_sky_blue    // Geometry::event_t::type::end
    };

    std::string format_neutral_text(std::string t, bool enable_color) {
      return format_col(enable_color, fmt::emphasis::faint | fg(fmt::color::peach_puff), "{}", t);
    }

    std::string format_event_type(Geometry::event_t::type tp, bool enable_color) {
      return format_col(enable_color, fmt::emphasis::bold | fg(type_col[tp]), "{}",
                (tp == Geometry::event_t::type::begin? "begin"
                  : tp == Geometry::event_t::type::interior? "interior" : "end"));
    }

    std::string format_id(size_t id, bool enable_color) {
      return format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::khaki), "{}", id);
    }

    std::string format_point(const Geometry::point_t &p, bool enable_color) {
      return format_col(enable_color, fmt::emphasis::faint | fg(fmt::color::medium_aquamarine),
              "({:.3f}, {:.3f})", p.x, p.y);
    }

    std::string format_event(const Geometry::event_t &e, bool enable_color) {
      return fmt::format("<{}, {}, {}>",
        format_point(e.p, enable_color),
        format_event_type(e.tp, enable_color),
        format_col(enable_color, fmt::emphasis::faint | fg(fmt::color::khaki),
          "id={}", format_id(e.seg_id + 1, enable_color))
      );
    }

    std::string format_segment(const Geometry::segment_t &s, bool enable_color) {
      return fmt::format("[{}, {}, {}]",
        format_point(s.p, enable_color),
        format_point(s.q, enable_color),
        format_col(enable_color, fmt::emphasis::faint | fg(fmt::color::khaki),
          "id={}", format_id(s.seg_id + 1, enable_color))
      );
    }

    std::string format_intersection(const intersection_t &it, bool enable_color) {
      std::string res = fmt::format("  {}  ", format_point(it.pt, enable_color));

      bool fst = true;
      for(int idx: it.segments)
        res += (fst? "" : ", ") + format_id(idx + 1, enable_color), fst = false;

      return res;
    }

  } // namespace detail

  Geometry::float_t Geometry::sweepline_X;

  std::vector<intersection_t> find_intersections(const std::vector<Geometry::segment_t> &line_segments, bool verbose, bool enable_color) {
    std::vector<intersection_t> result;
    Geometry::sweepline_X = -std::numeric_limits<Geometry::float_t>::max();

    if(verbose) {
      std::cerr << format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::orange), "line_segments") << " = {\n";
      for(auto &x: line_segments)
        std::cerr << '\t' << detail::format_segment(x, enable_color) << ',' << std::endl;
      std::cerr << '}' << std::endl << std::endl;
    }

    bbst<Geometry::event_t> event_queue;
    bbst<Geometry::segment_t> seg_ordering;
    std::vector<Geometry::segment_t> vertical_segs;

    size_t n = line_segments.size();
    for(size_t i = 0; i < n; i++) {
      const Geometry::point_t &p = line_segments[i].p;
      const Geometry::point_t &q = line_segments[i].q;

      if(std::fabs(p.x - q.x) < Geometry::EPS) {
        vertical_segs.emplace_back(line_segments[i]);
      } else {
        event_queue.insert({ p, Geometry::event_t::type::begin, i});
        event_queue.insert({ q, Geometry::event_t::type::end,   i});
      }
    }

    if(verbose) {
      std::cerr << format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::orange), "vertical_segs") << " = {";
      bool fst = false;
      for(auto &x: vertical_segs)
        std::cerr << (fst? "" : ", ") << detail::format_segment(x, enable_color), fst = false;
      std::cerr << '}' << std::endl;

      std::cerr << std::endl << "------------------------------------------------------------------------" << std::endl;
    }

    std::sort(vertical_segs.begin(), vertical_segs.end(),
      [](const Geometry::segment_t &a, const Geometry::segment_t &b) {
        return a.p.x == b.p.x? a.p.y < b.p.y : a.p.x < b.p.x;
      }
    );

    for(size_t i = 0; i + 1 < vertical_segs.size(); i++) {
      if(std::fabs(vertical_segs[i].q.y - vertical_segs[i + 1].p.y) < Geometry::EPS) {
        intersection_t it { vertical_segs[i].q,
                  std::vector<size_t>{ vertical_segs[i].seg_id, vertical_segs[i + 1].seg_id } };
        result.emplace_back(it);

        if(verbose) {
          std::cerr << detail::format_neutral_text("\nFound vertical-vertical segment intersection!\n", enable_color);
          std::cerr << detail::format_intersection(it, enable_color) << std::endl;
        }
      }
    }

    size_t vert_idx = 0;

    while(!event_queue.empty()) {
      Geometry::event_t top = *event_queue.begin();
      event_queue.erase(event_queue.begin());

      if(top.p.x < Geometry::sweepline_X) {
        if(verbose) {
          std::cerr << detail::format_neutral_text("top.p.x", enable_color) << " = " << top.p.x << std::endl;
          std::cerr << format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::green_yellow), "sweepline_X")
                      << " = " << Geometry::sweepline_X << std::endl;
          std::cerr << detail::format_neutral_text("continuing...\n", enable_color);
          std::cerr << std::endl << "------------------------------------------------------------------------" << std::endl;
        }
        continue;
      }

      while(vert_idx < vertical_segs.size()
        and vertical_segs[vert_idx].p.x < Geometry::sweepline_X - Geometry::EPS) {
          vert_idx++;
      }

      while(vert_idx < vertical_segs.size()
        and vertical_segs[vert_idx].p.x <= top.p.x + Geometry::EPS) {
          auto &vseg = vertical_segs[vert_idx];
          Geometry::sweepline_X = vseg.p.x;
          auto it = seg_ordering.lower_bound(Geometry::segment_t{ vseg.p, vseg.p, 0 });
          while(it != seg_ordering.end()) {
            Geometry::float_t it_y = it->eval_y(Geometry::sweepline_X);
            if(it_y > vseg.q.y + Geometry::EPS) break;
            intersection_t itrsctn { Geometry::point_t{ Geometry::sweepline_X, it_y },
                      std::vector<size_t>{ it->seg_id, vseg.seg_id } };
            result.emplace_back(itrsctn);
            if(verbose) {
              std::cerr << detail::format_neutral_text("\nFound vertical-non vertical segment intersection!\n", enable_color);
              std::cerr << detail::format_intersection(itrsctn, enable_color) << std::endl;
            }
            ++it;
          }

          vert_idx++;
      }

      Geometry::sweepline_X = top.p.x;

      if(verbose) {
        std::cerr << format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::green_yellow), "sweepline_X")
                    << " = " << Geometry::sweepline_X << std::endl << std::endl;

        std::cerr << detail::format_neutral_text("initially:\n", enable_color);

        std::cerr << format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::orange), "event_queue") << " = {";
        std::cerr << detail::format_event(top, enable_color);
        for(auto &x: event_queue)
          std::cerr << ", " << detail::format_event(x, enable_color);
        std::cerr << '}' << std::endl;

        std::cerr << format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::orange), "segment_ordering") << " = {";
        {
          bool fst = true;
          for(auto &x: seg_ordering)
            std::cerr << (fst? "" : ", ") << detail::format_id(x.seg_id + 1, enable_color), fst = false;
          std::cerr << '}' << std::endl;
        }
        std::cerr << std::endl;
      }

      std::array<std::vector<size_t>, 3> active;
      active[top.tp].push_back(top.seg_id);

      while(!event_queue.empty()
          and std::fabs(event_queue.begin()->p.x - top.p.x) < Geometry::EPS
          and std::fabs(event_queue.begin()->p.y - top.p.y) < Geometry::EPS) {

        Geometry::event_t nxt_top = *event_queue.begin();
        event_queue.erase(event_queue.begin());
        active[nxt_top.tp].push_back(nxt_top.seg_id);
      }

      if(verbose) {
        std::cerr << format_col(enable_color, fg(fmt::color::gold), "active point")
                    << " -> " << detail::format_point(top.p, enable_color) << std::endl;
        {
          std::cerr << fmt::format("{} {} {}      = {{",
                          detail::format_neutral_text("segments with", enable_color),
                          detail::format_event_type(Geometry::event_t::type::end, enable_color),
                          detail::format_neutral_text("point here", enable_color));
          bool fst = true;
          for(auto &x: active[Geometry::event_t::type::end])
            std::cerr << (fst? "" : ", ") << detail::format_id(x + 1, enable_color), fst = false;
          std::cerr << '}' << std::endl;
        }

        {
          std::cerr << fmt::format("{} {} {} = {{",
                          detail::format_neutral_text("segments with", enable_color),
                          detail::format_event_type(Geometry::event_t::type::interior, enable_color),
                          detail::format_neutral_text("point here", enable_color));
          bool fst = true;
          for(auto &x: active[Geometry::event_t::type::interior])
            std::cerr << (fst? "" : ", ") << detail::format_id(x + 1, enable_color), fst = false;
          std::cerr << '}' << std::endl;
        }

        {
          std::cerr << fmt::format("{} {} {}    = {{",
                          detail::format_neutral_text("segments with", enable_color),
                          detail::format_event_type(Geometry::event_t::type::begin, enable_color),
                          detail::format_neutral_text("point here", enable_color));
          bool fst = true;
          for(auto &x: active[Geometry::event_t::type::begin])
            std::cerr << (fst? "" : ", ") << detail::format_id(x + 1, enable_color), fst = false;
          std::cerr << '}' << std::endl;
        }
      }

      for(int idx: active[Geometry::event_t::type::end])
        seg_ordering.erase(line_segments[idx]);

      for(int idx: active[Geometry::event_t::type::interior])
        seg_ordering.erase(line_segments[idx]);

      Geometry::sweepline_X += Geometry::EPS_INC;
      Geometry::float_t mx = -std::numeric_limits<Geometry::float_t>::max();
      Geometry::float_t mn = std::numeric_limits<Geometry::float_t>::max();

      for(int idx: active[Geometry::event_t::type::begin]) {
        mn = std::min(mn, line_segments[idx].eval_y(Geometry::sweepline_X));
        mx = std::max(mx, line_segments[idx].eval_y(Geometry::sweepline_X));
        seg_ordering.insert(line_segments[idx]);
      }

      for(int idx: active[Geometry::event_t::type::interior]) {
        mn = std::min(mn, line_segments[idx].eval_y(Geometry::sweepline_X));
        mx = std::max(mx, line_segments[idx].eval_y(Geometry::sweepline_X));
        seg_ordering.insert(line_segments[idx]);
      }

      if(active[Geometry::event_t::type::begin].size() + active[Geometry::event_t::type::interior].size() == 0) {
        auto b_right = seg_ordering.lower_bound(Geometry::segment_t{ top.p, top.p, 0 });
        if(b_right != seg_ordering.end() and b_right != seg_ordering.begin()) {
          auto b_left = b_right;
          --b_left;
          if(is_intersecting(*b_left, *b_right)) {
            Geometry::point_t pt = intersection_point(*b_left, *b_right);
            Geometry::event_t::type tp1 = b_left->p == pt? Geometry::event_t::type::begin : Geometry::event_t::type::interior;
            Geometry::event_t::type tp2 = b_right->p == pt? Geometry::event_t::type::begin : Geometry::event_t::type::interior;

            event_queue.insert(Geometry::event_t{ pt, tp1, b_left->seg_id  });
            event_queue.insert(Geometry::event_t{ pt, tp2, b_right->seg_id });
          }
        }
      } else {
        Geometry::point_t left { Geometry::sweepline_X, mn - 2 * Geometry::EPS }, right { Geometry::sweepline_X, mx + 2 * Geometry::EPS };
        auto b_right = seg_ordering.lower_bound(Geometry::segment_t{ right, right, 0 });
        auto s_left  = seg_ordering.lower_bound(Geometry::segment_t{ left,  left,  0 });

        if(b_right != seg_ordering.end() and b_right != seg_ordering.begin()) {
          auto s_right = b_right;
          --s_right;

          if(is_intersecting(*s_right, *b_right)) {
            Geometry::point_t pt = intersection_point(*s_right, *b_right);
            Geometry::event_t::type tp1 = s_right->p == pt? Geometry::event_t::type::begin : Geometry::event_t::type::interior;
            Geometry::event_t::type tp2 = b_right->p == pt? Geometry::event_t::type::begin : Geometry::event_t::type::interior;

            event_queue.insert(Geometry::event_t{ pt, tp1, s_right->seg_id });
            event_queue.insert(Geometry::event_t{ pt, tp2, b_right->seg_id });
          }
        }

        if(s_left != seg_ordering.end() and s_left != seg_ordering.begin()) {
          auto b_left = s_left;
          --b_left;

          if(is_intersecting(*b_left, *s_left)) {
            Geometry::point_t pt = intersection_point(*b_left, *s_left);
            Geometry::event_t::type tp1 = b_left->p == pt? Geometry::event_t::type::begin : Geometry::event_t::type::interior;
            Geometry::event_t::type tp2 = s_left->p == pt? Geometry::event_t::type::begin : Geometry::event_t::type::interior;

            event_queue.insert(Geometry::event_t{ pt, tp1, b_left->seg_id });
            event_queue.insert(Geometry::event_t{ pt, tp2, s_left->seg_id });
          }
        }
      }

      Geometry::sweepline_X -= Geometry::EPS_INC;

      // we have an intersection
      if(active[0].size() + active[1].size() + active[2].size() > 1) {
        active[0].insert(active[0].end(), active[1].begin(), active[1].end());
        active[0].insert(active[0].end(), active[2].begin(), active[2].end());

        result.emplace_back(intersection_t{ top.p, active[0] });

        if(verbose) {
          std::cerr << detail::format_neutral_text("\nFound intersection!\n", enable_color);
          std::cerr << detail::format_intersection(result.back(), enable_color) << std::endl;
        }
      }

      if(verbose) {
        std::cerr << detail::format_neutral_text("\nfinally:\n", enable_color);

        std::cerr << format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::orange), "event_queue") << " = {";
        {
          bool fst = true;
          for(auto &x: event_queue)
            std::cerr << (fst? "" : ", ") << detail::format_event(x, enable_color), fst = false;
          std::cerr << '}' << std::endl;
        }

        std::cerr << format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::orange), "segment_ordering") << " = {";
        {
          bool fst = true;
          for(auto &x: seg_ordering)
            std::cerr << (fst? "" : ", ") << detail::format_id(x.seg_id + 1, enable_color), fst = false;
          std::cerr << '}' << std::endl;
        }

        std::cerr << std::endl << "------------------------------------------------------------------------" << std::endl;
      }
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

    if(verbose)
      std::cerr << std::endl;

    return answer;
  }

} // namespace Sweepline