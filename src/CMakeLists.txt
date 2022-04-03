set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/sweepline_intersections/lib.hpp"
)

# headers are optional, but won't show up in IDEs unless listed
add_library(mylib STATIC lib.cpp "${HEADER_LIST}")

# PUBLIC because users of our library will need it too
target_include_directories(mylib PUBLIC ../include)

#   remove set(CMAKE_CXX_STANDARD xx) if you want target-wise standards
# target_compile_features(mylib PUBLIC cxx_std_xx)

# so IDEs put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)