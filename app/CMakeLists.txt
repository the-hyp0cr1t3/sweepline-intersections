add_executable(app app.cpp)

set_target_properties(app
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

target_link_libraries(app PRIVATE sweepline utils fmt::fmt)