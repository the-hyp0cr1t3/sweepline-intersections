# A sweepline algorithm to find line segment intersections {#mainpage}
**Problem:** Given \f$ n \f$ line segments, efficiently find intersection points in \f$ \mathcal{O}((n + k) log n) \f$ where \f$ k \f$ is the number of intersections.

This is a modern C++ implementation of the [Bentley-Ottman algorithm](https://en.wikipedia.org/wiki/Bentley%E2%80%93Ottmann_algorithm) from (almost) scratch because why not.

The main function is `::find_intersections()`.