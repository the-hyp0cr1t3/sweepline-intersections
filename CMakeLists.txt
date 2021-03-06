# Tested and works on Ubuntu 20.04
# Windows seems to have some issues, still trying to learn multiplatform

cmake_minimum_required(VERSION 3.17)

project(
  "Sweepline Intersections"
    VERSION 2.0
    LANGUAGES CXX
    DESCRIPTION "Given n line segments, efficiently find \
      all intersection points in O((n + k) log n) \
      where k is the number of intersections."
)

# only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # set required C++ standard
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    add_compile_options(
      # Release flags
      $<$<CONFIG:RELEASE>:-O3>

      # Debug flags
      $<$<CONFIG:DEBUG>:-O0>
      $<$<CONFIG:DEBUG>:-ggdb3>
    )

    add_compile_definitions(
      $<$<CONFIG:RELEASE>:NDEBUG>   # define NDEBUG in Release mode
    )

    # support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # docs only available if this is the main project
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()

    # enable creation and submission of dashboards and call enable_testing()
    include(CTest)

endif()

# facilitate downloads during the configure step
include(FetchContent)

# Formatting library
FetchContent_Declare(
  fmtlib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 8.1.1)
FetchContent_MakeAvailable(fmtlib)
# adds fmt::fmt

# generate utils.hpp with path variables substituted in #defines
configure_file (
  "${PROJECT_SOURCE_DIR}/include/utils/utils.hpp.in"
  "${PROJECT_BINARY_DIR}/include/utils/utils.hpp"
)

# compiled library code is here
add_subdirectory(src)

# executable code is here
add_subdirectory(app)

# testing only if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()