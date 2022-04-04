add_library(utils STATIC
    utils.cpp

    # headers are optional, but won't show up in IDEs unless listed
    "${CMAKE_SOURCE_DIR}/include/utils/utils.hpp"
)

target_include_directories(utils PUBLIC
    "${CMAKE_SOURCE_DIR}/include/utils"
)