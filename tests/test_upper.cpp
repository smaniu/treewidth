#include <iostream>
#include <sstream>

#include "catch.hpp"
#include "Graph.h"
#include "PermutationStrategy.h"
#include "DegreePermutationStrategy.h"
#include "FillInPermutationStrategy.h"
#include "DegreeFillInPermutationStrategy.h"
#include "MCSPermutationStrategy.h"
#include "TreeDecomposition.h"
#include "graph_builders.h"
#include "decomposition_check.h"

// decompose() is destructive, so each call takes the graph by value.
static unsigned long upper_tw(Graph g, PermutationStrategy& s) {
  TreeDecomposition dec(g, s);
  return dec.decompose();
}

TEST_CASE("upper bound is exact on structured graphs (min-degree)") {
  { DegreePermutationStrategy s; REQUIRE(upper_tw(clique(5), s) == 4); }
  { DegreePermutationStrategy s; REQUIRE(upper_tw(clique(2), s) == 1); }
  { DegreePermutationStrategy s; REQUIRE(upper_tw(path(6), s) == 1); }
  { DegreePermutationStrategy s; REQUIRE(upper_tw(cycle(5), s) == 2); }
  { DegreePermutationStrategy s; REQUIRE(upper_tw(cycle(3), s) == 2); }
}

TEST_CASE("upper bound never exceeds N-1 (N = number of nodes)") {
  for (unsigned long n = 2; n <= 8; n++) {
    DegreePermutationStrategy s;
    REQUIRE(upper_tw(grid(n, 3), s) <= 3 * n - 1);
  }
}

// Golden treewidth values (first line of the .dec output) pinned per method on
// the CLI baseline graph. Method 3 (MCS) used to run through a by-value
// recompute() override that silently fell back to the base-class statistic
// (a no-op override bug); MCSPermutationStrategy::recompute now genuinely
// overrides the base and the MCS-specific logic is live. The pinned width of
// 3 was verified unchanged across the fix (ties break the same way), so this
// remains the correct golden value -- it is no longer pinning known-buggy
// behavior, it is pinning the fixed, intended behavior.
TEST_CASE("golden treewidth per method on the sample graph") {
  { DegreePermutationStrategy s;       REQUIRE(upper_tw(sample_two_triangles(), s) == 2); }
  { FillInPermutationStrategy s;       REQUIRE(upper_tw(sample_two_triangles(), s) == 2); }
  { DegreeFillInPermutationStrategy s; REQUIRE(upper_tw(sample_two_triangles(), s) == 2); }
  { MCSPermutationStrategy s;          REQUIRE(upper_tw(sample_two_triangles(), s) == 3); }
}

TEST_CASE("min-degree output is a valid tree decomposition") {
  auto check = [](Graph g) {
    auto edges = collect_edges(g);
    auto verts = collect_vertices(g);
    DegreePermutationStrategy s;
    TreeDecomposition dec(g, s);
    dec.decompose();
    dec.build_tree();
    std::ostringstream ss;
    ss << dec;
    ParsedDecomp parsed = parse_decomposition(ss.str());
    REQUIRE(is_valid_tree_decomposition(parsed, edges, verts));
  };
  check(clique(5));
  check(path(6));
  check(cycle(6));
  check(grid(3, 3));
  check(sample_two_triangles());
}
