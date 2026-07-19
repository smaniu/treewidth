#include <iostream>
#include <vector>

#include "catch.hpp"
#include "Graph.h"
#include "DegreePermutationStrategy.h"
#include "LowerBound.h"
#include "LowerBoundMMD.h"
#include "Delta2D.h"
#include "LBN.h"
#include "LBNPlus.h"
#include "TreeDecomposition.h"
#include "graph_builders.h"

// Delta2D and the meta-heuristics (LBN / LBN+) used to be excluded here:
// Delta2D queried get_neighbours() for an already-removed node (assertion in
// a debug build, SIGSEGV under NDEBUG, because the strategy queue and
// graph_temp had desynced across estimateForNode() calls), and LBN/LBN+ drove
// the buggy CE contraction (contracted on every neighbour-selection loop
// iteration instead of once after it) and the isolated-node get_neighbours UB.
// All four fixes are now in place (PermutationStrategy::clear(), the
// get_neighbours empty-set fallback, the genuine MCS recompute() override,
// and CE::con_edge() contracting once after its loop), so these paths are
// exercised below.

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

// --- Delta2D (bug 2: strategy-queue / graph_temp desync, SIGSEGV under
// NDEBUG) -------------------------------------------------------------

TEST_CASE("Delta2D lower bound is between 1 and the greedy upper bound") {
  std::vector<Graph> graphs{clique(5), cycle(6), grid(3, 3),
                            sample_two_triangles()};
  for (auto& base : graphs) {
    unsigned long up = upper_tw(base);
    Graph g = base;
    DegreePermutationStrategy s;
    Delta2D lb(g, s);
    unsigned long lo = lb.estimate();
    REQUIRE(lo >= 1);
    REQUIRE(lo <= up);
  }
}

TEST_CASE("Delta2D lower bound equals the greedy upper bound on a clique") {
  // A discriminating case: on clique(5) Delta2D's bound is tight (4), equal
  // to the greedy min-degree upper bound, rather than a loose lower value.
  Graph g = clique(5);
  DegreePermutationStrategy s;
  Delta2D lb(g, s);
  REQUIRE(lb.estimate() == 4);
}

// --- LBN / LBN+ meta lower bounds (bug 1: isolated-node get_neighbours UB
// reachable via neighbour_improved(); bug 4: CE double-contraction) --------
//
// LBN/LBN+ hold their own Graph& alongside the wrapped LowerBound's Graph&,
// and both are bound to the very same Graph object here -- mirroring how
// src/main.cpp wires `new LBN(graph, *bounds[lb])` with `bounds[lb]` already
// constructed on that identical `graph`. LowerBound::setGraph() assigns into
// the referenced object rather than rebinding the reference, so a *destructive*
// base bound (LowerBoundMMD/LowerBoundMMDPlus, which remove/contract nodes out
// of its own graph reference as it runs) empties the shared graph before the
// meta-heuristic's neighbour-improvement loop -- and LBN+'s CE contraction --
// ever get a chance to run on real edges. Delta2D does not mutate its own
// graph reference (it works on an internal copy), so using it as the base
// bound is what actually drives the improvement loop and the fixed CE path.

TEST_CASE("LBN meta lower bound (Delta2D base) is between 1 and the upper bound") {
  std::vector<Graph> graphs{clique(5), cycle(6), grid(3, 3),
                            sample_two_triangles()};
  for (auto& base : graphs) {
    unsigned long up = upper_tw(base);
    Graph g = base;
    DegreePermutationStrategy s;
    Delta2D base_lb(g, s);
    LBN lbn(g, base_lb);
    unsigned long lo = lbn.estimate();
    REQUIRE(lo >= 1);
    REQUIRE(lo <= up);
  }
}

TEST_CASE("LBN+ meta lower bound (Delta2D base) is between 1 and the upper bound") {
  std::vector<Graph> graphs{clique(5), cycle(6), grid(3, 3),
                            sample_two_triangles()};
  for (auto& base : graphs) {
    unsigned long up = upper_tw(base);
    Graph g = base;
    DegreePermutationStrategy s;
    Delta2D base_lb(g, s);
    LBNPlus lbnp(g, base_lb);
    unsigned long lo = lbnp.estimate();
    REQUIRE(lo >= 1);
    REQUIRE(lo <= up);
  }
}

TEST_CASE("LBN meta lower bound (MMD base) is between 1 and the upper bound") {
  // MMD is destructive to the shared graph reference (see note above), so
  // this exercises LBN's degenerate-but-safe path: no crash, and the
  // invariant still holds even though the improvement loop can't add edges.
  std::vector<Graph> graphs{clique(5), cycle(6), grid(3, 3),
                            sample_two_triangles()};
  for (auto& base : graphs) {
    unsigned long up = upper_tw(base);
    Graph g = base;
    DegreePermutationStrategy s;
    LowerBoundMMD base_lb(g, s);
    LBN lbn(g, base_lb);
    unsigned long lo = lbn.estimate();
    REQUIRE(lo >= 1);
    REQUIRE(lo <= up);
  }
}
