add_executable(app app.cpp)

#   remove set(CMAKE_CXX_STANDARD xx) if you want target-wise standards
# target_compile_features(app PRIVATE cxx_std_xx)

set_target_properties(app
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

target_link_libraries(app PRIVATE mylib fmt::fmt)