#include <sweepline.hpp>

#include <fmt/format.h>
#include <fmt/color.h>
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>

#define format_col(enable_color, ts, argn...) \
  fmt::format((enable_color? ts : fmt::v8::text_style()), argn)

// This namespace is meant to be hidden from the API
// provides implementation of debugging utility functions
namespace detail {
  bool enable_color = true;

  std::array<fmt::color, 3> type_col {
    fmt::color::light_sea_green,  // sweepline::event_t::type::begin
    fmt::color::medium_purple,    // sweepline::event_t::type::interior
    fmt::color::light_sky_blue    // sweepline::event_t::type::end
  };

  std::string format_neutral_text(std::string t) {
    return format_col(enable_color, fmt::emphasis::faint | fg(fmt::color::peach_puff), "{}", t);
  }

  std::string format_heading_text(std::string t) {
    return format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::green_yellow), "{}", t);
  }

  std::string format_subheading_text(std::string t) {
    return format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::orange), "{}", t);
  }

  std::string format_event_type(sweepline::event_t::type tp) {
    return format_col(enable_color, fmt::emphasis::bold | fg(type_col[tp]), "{}",
              (tp == sweepline::event_t::type::begin? "begin"
                : tp == sweepline::event_t::type::interior? "interior" : "end"));
  }

  std::string format_id(size_t id) {
    return format_col(enable_color, fmt::emphasis::bold | fg(fmt::color::khaki), "{}", id);
  }

  std::string format_point(const geometry::point_t &p) {
    return format_col(enable_color, fmt::emphasis::faint | fg(fmt::color::medium_aquamarine),
            "({:.3f}, {:.3f})", p.x, p.y);
  }

  std::string format_event(const sweepline::event_t &e) {
    return fmt::format("<{}, {}, {}>",
      format_point(e.p),
      format_event_type(e.tp),
      format_col(enable_color, fmt::emphasis::faint | fg(fmt::color::khaki),
        "id={}", format_id(e.seg_id + 1))
    );
  }

  std::string format_segment(const geometry::segment_t &s) {
    return fmt::format("[{}, {}, {}]",
      format_point(s.p),
      format_point(s.q),
      format_col(enable_color, fmt::emphasis::faint | fg(fmt::color::khaki),
        "id={}", format_id(s.seg_id + 1))
    );
  }

  std::string format_intersection(const sweepline::intersection_t &it) {
    std::string res = fmt::format("  {}  ", format_point(it.pt));

    bool fst = true;
    for(int idx: it.segments)
      res += (fst? "" : ", ") + format_id(idx + 1), fst = false;

    return res;
  }

  void debug_line() {
    std::cerr << std::endl << "------------------------------------------------------------------------" << std::endl;
  }

  void debug_continuing() {
    std::cerr << detail::format_neutral_text("continuing...\n");
    debug_line();
  }

  void debug_line_segments(const std::vector<geometry::segment_t> &line_segments) {
    std::cerr << detail::format_subheading_text("line_segments") << " = {\n";
    for(auto &x: line_segments)
      std::cerr << '\t' << detail::format_segment(x) << ',' << std::endl;
    std::cerr << '}' << std::endl << std::endl;
  }

  void debug_vertical_segs(const std::vector<geometry::segment_t> &vertical_segs) {
    std::cerr << detail::format_subheading_text("vertical_segs") << " = {";
    bool fst = false;
    for(auto &x: vertical_segs)
      std::cerr << (fst? "" : ", ") << detail::format_segment(x), fst = false;
    std::cerr << '}' << std::endl;
  }

  void debug_intersection(const sweepline::intersection_t &it, std::string type = "") {
    std::cerr << detail::format_neutral_text("\nFound " + type + " intersection!\n");
    std::cerr << detail::format_intersection(it) << std::endl;
  }

  void debug_initial(
    geometry::float_t sweeplineX,
    sweepline::event_t top,
    const sweepline::bbst<sweepline::event_t> &event_queue,
    const sweepline::bbst<geometry::segment_t> &seg_ordering) {

    std::cerr << detail::format_heading_text("sweeplineX") << " = " << sweeplineX << std::endl << std::endl;
    std::cerr << detail::format_neutral_text("initially:\n");

    std::cerr << detail::format_subheading_text("event_queue") << " = {";
    std::cerr << detail::format_event(top);
    for(auto &x: event_queue)
      std::cerr << ", " << detail::format_event(x);
    std::cerr << '}' << std::endl;

    std::cerr << detail::format_subheading_text("segment_ordering") << " = {";
    {
      bool fst = true;
      for(auto &x: seg_ordering)
        std::cerr << (fst? "" : ", ") << detail::format_id(x.seg_id + 1), fst = false;
      std::cerr << '}' << std::endl;
    }
    std::cerr << std::endl;
  }

  void debug_final(
    const sweepline::bbst<sweepline::event_t> &event_queue,
    const sweepline::bbst<geometry::segment_t> &seg_ordering) {

    std::cerr << detail::format_neutral_text("\nfinally:\n");

    std::cerr << detail::format_subheading_text("event_queue") << " = {";
    {
      bool fst = true;
      for(auto &x: event_queue)
        std::cerr << (fst? "" : ", ") << detail::format_event(x), fst = false;
      std::cerr << '}' << std::endl;
    }

    std::cerr << detail::format_subheading_text("segment_ordering") << " = {";
    {
      bool fst = true;
      for(auto &x: seg_ordering)
        std::cerr << (fst? "" : ", ") << detail::format_id(x.seg_id + 1), fst = false;
      std::cerr << '}' << std::endl;
    }

    debug_line();
  }

  void debug_active_events(sweepline::event_t top, const std::array<std::vector<size_t>, 3> &active) {
    std::cerr << format_col(enable_color, fg(fmt::color::gold), "active point")
                << " -> " << detail::format_point(top.p) << std::endl;
    {
      std::cerr << fmt::format("{} {} {}      = {{",
                      detail::format_neutral_text("segments with"),
                      detail::format_event_type(sweepline::event_t::type::end),
                      detail::format_neutral_text("point here"));
      bool fst = true;
      for(auto &x: active[sweepline::event_t::type::end])
        std::cerr << (fst? "" : ", ") << detail::format_id(x + 1), fst = false;
      std::cerr << '}' << std::endl;
    }

    {
      std::cerr << fmt::format("{} {} {} = {{",
                      detail::format_neutral_text("segments with"),
                      detail::format_event_type(sweepline::event_t::type::interior),
                      detail::format_neutral_text("point here"));
      bool fst = true;
      for(auto &x: active[sweepline::event_t::type::interior])
        std::cerr << (fst? "" : ", ") << detail::format_id(x + 1), fst = false;
      std::cerr << '}' << std::endl;
    }

    {
      std::cerr << fmt::format("{} {} {}    = {{",
                      detail::format_neutral_text("segments with"),
                      detail::format_event_type(sweepline::event_t::type::begin),
                      detail::format_neutral_text("point here"));
      bool fst = true;
      for(auto &x: active[sweepline::event_t::type::begin])
        std::cerr << (fst? "" : ", ") << detail::format_id(x + 1), fst = false;
      std::cerr << '}' << std::endl;
    }

  }

} // namespace detail

std::vector<sweepline::intersection_t> sweepline::find_intersections(
  const std::vector<geometry::segment_t> &line_segments,
  bool verbose,
  bool enable_color
) {

  return sweepline::solver(line_segments, verbose, enable_color).solve();
}

geometry::float_t sweepline::sweeplineX;

sweepline::solver::solver(const std::vector<geometry::segment_t> &line_segments, bool verbose, bool enable_color)
  : line_segments(line_segments), verbose(verbose) {

    detail::enable_color = enable_color;  // set/unset color printing
}

std::vector<sweepline::intersection_t> sweepline::solver::solve() {
  // initialize the sweepline to -inf
  sweepline::sweeplineX = -std::numeric_limits<geometry::float_t>::max();

  // initialize the event_queue by inserting the end points of the line segments
  // and populate vertical_segs with vertical segments
  init_event_queue();

  // sort vertical segments by x then y
  std::sort(vertical_segs.begin(), vertical_segs.end(),
    [](const geometry::segment_t &a, const geometry::segment_t &b) {
      return a.p.x == b.p.x? a.p.y < b.p.y : a.p.x < b.p.x;
    }
  );

  // find intersections between pairs of vertical line segments
  find_vertical_vertical_intersections();

  while(!event_queue.empty()) {
    sweepline::event_t top = *event_queue.begin();
    event_queue.erase(event_queue.begin());

    if(top.p.x < sweepline::sweeplineX) {
      if(verbose)
        detail::debug_continuing();

      continue;
    }

    // find intersections between a vertical line segment and one or more non-vertical segments
    find_vertical_nonvertical_intersections(top.p.x);

    // move sweepline to x coordinate of event being processed
    sweepline::sweeplineX = top.p.x;

    if(verbose)
      detail::debug_initial(sweepline::sweeplineX, top, event_queue, seg_ordering);

    // get the active segments with an event at the point currently being processed
    // returns three arrays of active segment indices corresponding to event_t::type
    auto active_segs = get_active_segs(top);

    // remove all end points, insert all begin points and reorder the interior points
    update_segment_ordering(active_segs);

    // if no segments were newly inserted, the immediate left and right neighbours
    // of the deleted set of segments become adjacent candidates for intersection
    if(active_segs[sweepline::event_t::type::begin].size()
        + active_segs[sweepline::event_t::type::interior].size() == 0)
            handle_no_newly_inserted(top.p);
    else
      handle_extremes_of_newly_inserted();
    // else the left and right extremes among the set of newly inserted segments
    // must be checked for intersection with the immediate left and right neighbours respectively

    // reset the sweepline to the x coordinate of event being processed
    sweepline::sweeplineX = top.p.x;

    // finally report the union of all active segments as an intersection if there are two or more of them
    if(active_segs[0].size() + active_segs[1].size() + active_segs[2].size() > 1)
      report_intersection(top.p, std::move(active_segs));

    if(verbose)
      detail::debug_final(event_queue, seg_ordering);
  }

  merge_intersection_points();

  if(verbose)
    std::cerr << std::endl;

  return result;
}

void sweepline::solver::init_event_queue() {
  if(verbose)
    detail::debug_line_segments(line_segments);

  for(size_t i = 0; i < line_segments.size(); i++) {
    const auto &p = line_segments[i].p;
    const auto &q = line_segments[i].q;

    if(std::fabs(p.x - q.x) < geometry::EPS) {
      // handle (vertical) segments with same slope as sweepline separately
      vertical_segs.emplace_back(line_segments[i]);
    } else {
      // insert the begin and end points of each segment in the event queue
      event_queue.insert({ p, sweepline::event_t::type::begin, i});
      event_queue.insert({ q, sweepline::event_t::type::end,   i});
    }
  }

  if(verbose) {
    detail::debug_vertical_segs(vertical_segs);
    detail::debug_line();
  }
}

void sweepline::solver::find_vertical_vertical_intersections() {
  for(size_t i = 0; i + 1 < vertical_segs.size(); i++) {
    if(vertical_segs[i].q == vertical_segs[i + 1].p) {
      sweepline::intersection_t it {
        vertical_segs[i].q,
        std::vector<size_t> {
          vertical_segs[i].seg_id,
          vertical_segs[i + 1].seg_id
        }
      };

      if(verbose)
        detail::debug_intersection(it, "vertical<->vertical segment");

      result.emplace_back(it);
    }
  }
}

void sweepline::solver::find_vertical_nonvertical_intersections(geometry::float_t max_vsegx) {
  while(vert_idx < vertical_segs.size()
    and vertical_segs[vert_idx].p.x < sweepline::sweeplineX - geometry::EPS)
      vert_idx++;

  while(vert_idx < vertical_segs.size()
    and vertical_segs[vert_idx].p.x <= max_vsegx + geometry::EPS) {

      auto &vseg = vertical_segs[vert_idx];
      sweepline::sweeplineX = vseg.p.x;

      auto itr = seg_ordering.lower_bound(geometry::segment_t{ vseg.p, vseg.p, 0 });

      while(itr != seg_ordering.end()) {
        geometry::float_t it_y = itr->eval_y(sweepline::sweeplineX);

        if(it_y > vseg.q.y + geometry::EPS)
          break;

        sweepline::intersection_t it {
          geometry::point_t{ sweepline::sweeplineX, it_y },
          std::vector<size_t>{ itr->seg_id, vseg.seg_id }
        };

        if(verbose)
          detail::debug_intersection(it, "vertical<->non-vertical segment");

        result.emplace_back(it);

        ++itr;
      }

      vert_idx++;
  }
}

std::array<std::vector<size_t>, 3> sweepline::solver::get_active_segs(sweepline::event_t top) {
  // array of all segments with an event at the point currently being processed
  //   active[event_t::type::begin]    -> list of segments which begin at this point
  //   active[event_t::type::interior] -> list of segments which intersect with some other segment at this point
  //   active[event_t::type::end]      -> list of segments which end at this point
  std::array<std::vector<size_t>, 3> active;
  active[top.tp].push_back(top.seg_id);

  // get all segments with an event at top.p and add them to one of the above
  while(!event_queue.empty() and event_queue.begin()->p == top.p) {
    sweepline::event_t nxt_top = *event_queue.begin();
    event_queue.erase(event_queue.begin());
    active[nxt_top.tp].push_back(nxt_top.seg_id);
  }

  if(verbose)
    detail::debug_active_events(top, active);

  return active;
}

void sweepline::solver::update_segment_ordering(const std::array<std::vector<size_t>, 3> &active) {
  // remove all end event segments
  for(size_t idx: active[sweepline::event_t::type::end])
    seg_ordering.erase(line_segments[idx]);

  // remove all interior event segments
  for(size_t idx: active[sweepline::event_t::type::interior])
    seg_ordering.erase(line_segments[idx]);

  // increment the sweepline by a very small amount, just past the intersection point
  sweepline::sweeplineX += geometry::EPS_INC;

  max_y = -std::numeric_limits<geometry::float_t>::max();
  min_y = std::numeric_limits<geometry::float_t>::max();

  // insert all begin type events
  for(int idx: active[sweepline::event_t::type::begin]) {
    min_y = std::min(min_y, line_segments[idx].eval_y(sweepline::sweeplineX));
    max_y = std::max(max_y, line_segments[idx].eval_y(sweepline::sweeplineX));
    seg_ordering.insert(line_segments[idx]);
  }

  // re-insert all interior type events (so that ordering is updated)
  for(int idx: active[sweepline::event_t::type::interior]) {
    min_y = std::min(min_y, line_segments[idx].eval_y(sweepline::sweeplineX));
    max_y = std::max(max_y, line_segments[idx].eval_y(sweepline::sweeplineX));
    seg_ordering.insert(line_segments[idx]);
  }
}

void sweepline::solver::handle_no_newly_inserted(geometry::point_t cur) {
  auto b_right = seg_ordering.lower_bound(geometry::segment_t{ cur, cur, 0 });
  if(b_right != seg_ordering.end() and b_right != seg_ordering.begin()) {
    auto b_left = b_right;
    --b_left;
    if(geometry::is_intersecting(*b_left, *b_right)) {
      geometry::point_t pt = geometry::intersection_point(*b_left, *b_right);
      sweepline::event_t::type tp1 = b_left->p == pt? sweepline::event_t::type::begin : sweepline::event_t::type::interior;
      sweepline::event_t::type tp2 = b_right->p == pt? sweepline::event_t::type::begin : sweepline::event_t::type::interior;

      event_queue.insert(sweepline::event_t{ pt, tp1, b_left->seg_id  });
      event_queue.insert(sweepline::event_t{ pt, tp2, b_right->seg_id });
    }
  }
}

void sweepline::solver::handle_extremes_of_newly_inserted() {
  geometry::point_t left { sweepline::sweeplineX, min_y - 2 * geometry::EPS }, right { sweepline::sweeplineX, max_y + 2 * geometry::EPS };
  auto b_right = seg_ordering.lower_bound(geometry::segment_t{ right, right, 0 });
  auto s_left  = seg_ordering.lower_bound(geometry::segment_t{ left,  left,  0 });

  // check for candidate intersection at the right extreme
  if(b_right != seg_ordering.end() and b_right != seg_ordering.begin()) {
    auto s_right = b_right;
    --s_right;

    if(is_intersecting(*s_right, *b_right)) {
      geometry::point_t pt = intersection_point(*s_right, *b_right);
      sweepline::event_t::type tp1 = s_right->p == pt? sweepline::event_t::type::begin : sweepline::event_t::type::interior;
      sweepline::event_t::type tp2 = b_right->p == pt? sweepline::event_t::type::begin : sweepline::event_t::type::interior;

      event_queue.insert(sweepline::event_t{ pt, tp1, s_right->seg_id });
      event_queue.insert(sweepline::event_t{ pt, tp2, b_right->seg_id });
    }
  }

  // check for candidate intersection at the left extreme
  if(s_left != seg_ordering.end() and s_left != seg_ordering.begin()) {
    auto b_left = s_left;
    --b_left;

    if(is_intersecting(*b_left, *s_left)) {
      geometry::point_t pt = intersection_point(*b_left, *s_left);
      sweepline::event_t::type tp1 = b_left->p == pt? sweepline::event_t::type::begin : sweepline::event_t::type::interior;
      sweepline::event_t::type tp2 = s_left->p == pt? sweepline::event_t::type::begin : sweepline::event_t::type::interior;

      event_queue.insert(sweepline::event_t{ pt, tp1, b_left->seg_id });
      event_queue.insert(sweepline::event_t{ pt, tp2, s_left->seg_id });
    }
  }
}

void sweepline::solver::report_intersection(geometry::point_t cur, std::array<std::vector<size_t>, 3> &&active) {
  active[0].insert(active[0].end(), active[1].begin(), active[1].end());
  active[0].insert(active[0].end(), active[2].begin(), active[2].end());

  result.emplace_back(sweepline::intersection_t{ cur, std::move(active[0]) });

  if(verbose)
    detail::debug_intersection(result.back());
}

void sweepline::solver::merge_intersection_points() {
  std::sort(result.begin(), result.end(),
    [](const sweepline::intersection_t &a, const sweepline::intersection_t &b) {
      return a.pt.x == b.pt.x? a.pt.y < b.pt.y : a.pt.x < b.pt.x;
    }
  );

  std::vector<sweepline::intersection_t> merged;
  for(size_t i = 0, j; i < result.size(); i = j) {
    std::vector<size_t> indices;
    for(j = i; j < result.size() and result[j].pt == result[i].pt; j++)
      indices.insert(indices.end(), result[j].segments.begin(), result[j].segments.end());

    std::sort(indices.begin(), indices.end());
    indices.erase(std::unique(indices.begin(), indices.end()), indices.end());

    merged.emplace_back(
      sweepline::intersection_t {
        result[i].pt, indices
      }
    );
  }

  result = std::move(merged);
}