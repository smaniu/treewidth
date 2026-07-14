# Vendored Catch2 v2 single-header, exposed as an INTERFACE target.
add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(treewidth_tests
  catch_main.cpp
  test_graph.cpp
  test_upper.cpp
  test_lower.cpp
  test_serialization.cpp)
target_link_libraries(treewidth_tests PRIVATE treewidth_lib catch2)

add_test(NAME treewidth_tests COMMAND treewidth_tests)
