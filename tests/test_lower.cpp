#include <iostream>
#include <vector>

#include "catch.hpp"
#include "Graph.h"
#include "DegreePermutationStrategy.h"
#include "LowerBound.h"
#include "LowerBoundMMD.h"
#include "TreeDecomposition.h"
#include "graph_builders.h"

// Delta2D and the meta-heuristics (LBN / LBN+) are intentionally excluded here:
// Delta2D queries get_neighbours() for an already-removed node (assertion in a
// debug build, SIGSEGV under NDEBUG), and LBN/LBN+ drive the known-buggy CE
// contraction and the isolated-node get_neighbours UB. These paths are covered
// once those bugs are fixed. See CLAUDE.md.

static unsigned long upper_tw(Graph g) {
  DegreePermutationStrategy s;
  TreeDecomposition dec(g, s);
  return dec.decompose();
}

TEST_CASE("lower bounds never exceed the greedy upper bound") {
  std::vector<Graph> graphs{clique(5), cycle(6), grid(3, 3),
                            sample_two_triangles()};
  for (auto& base : graphs) {
    unsigned long up = upper_tw(base);
    {
      Graph g = base;
      DegreePermutationStrategy s;
      LowerBoundMMD lb(g, s);
      unsigned long lo = lb.estimate();
      REQUIRE(lo >= 1);
      REQUIRE(lo <= up);
    }
    {
      Graph g = base;
      DegreePermutationStrategy s;
      LowerBoundMMDPlus lb(g, s);
      REQUIRE(lb.estimate() <= up);
    }
  }
}

TEST_CASE("MMD lower bound equals n-1 for a clique") {
  Graph g = clique(5);
  DegreePermutationStrategy s;
  LowerBoundMMD lb(g, s);
  REQUIRE(lb.estimate() == 4);
}
