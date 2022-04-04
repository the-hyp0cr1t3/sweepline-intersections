add_executable(app app.cpp utils.cpp)

target_include_directories(app PRIVATE
  "${PROJECT_BINARY_DIR}/include/utils"
  "${CMAKE_SOURCE_DIR}/extern"
)

target_link_libraries(app PRIVATE sweepline fmt::fmt)

set_target_properties(app
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)