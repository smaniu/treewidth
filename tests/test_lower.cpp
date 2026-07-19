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

// --- LBN / LBN+ meta lower bounds ---------------------------------------
//
// LBN/LBN+ hold their own Graph& alongside the wrapped LowerBound, both bound
// to the same Graph object here -- mirroring src/main.cpp's
// `new LBN(graph, *bounds[lb])`. LowerBound::graph is now a rebindable pointer,
// so setGraph() repoints the base bound at the throwaway copy a meta-heuristic
// hands it; a destructive base (LowerBoundMMD/LowerBoundMMDPlus) destroys that
// copy rather than the meta's shared graph, and the neighbour-improvement loop
// (and LBN+'s CE contraction) run on real edges. Previously setGraph()
// copy-assigned into the shared graph, emptying it and making the MMD/MMD+
// based meta bounds silent no-ops (fixed; see the improvement test below).

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
  // With the setGraph() fix the destructive MMD base no longer empties the
  // meta's shared graph, so this drives LBN's improvement loop on real edges.
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

// Regression anchor for the setGraph() no-op fix: with a *destructive* MMD
// base, LBN+ must be able to strictly improve on the base bound rather than
// silently returning it. On the 4x4 grid (treewidth 4) the plain MMD bound is
// 2; the fixed LBN+ neighbour-improvement + contraction reaches the exact
// treewidth 4. Before the fix this returned the base value (2), unchanged.
TEST_CASE("LBN+ meta lower bound (MMD base) improves over the base bound") {
  Graph grid44 = grid(4, 4);
  unsigned long up = upper_tw(grid44);

  unsigned long base_val;
  {
    Graph g = grid44;
    DegreePermutationStrategy s;
    LowerBoundMMD base_lb(g, s);
    base_val = base_lb.estimate();
  }

  Graph g = grid44;
  DegreePermutationStrategy s;
  LowerBoundMMD base_lb(g, s);
  LBNPlus lbnp(g, base_lb);
  unsigned long meta_val = lbnp.estimate();

  REQUIRE(meta_val > base_val);   // fix active: not a silent no-op
  REQUIRE(meta_val <= up);        // still a valid lower bound
}

// LBN+ with the destructive MMD+ base must also stay valid (<= upper) on the
// standard graphs now that it runs on real edges.
TEST_CASE("LBN+ meta lower bound (MMD+ base) is between 1 and the upper bound") {
  std::vector<Graph> graphs{clique(5), cycle(6), grid(3, 3), grid(4, 4),
                            sample_two_triangles()};
  for (auto& base : graphs) {
    unsigned long up = upper_tw(base);
    Graph g = base;
    DegreePermutationStrategy s;
    LowerBoundMMDPlus base_lb(g, s);
    LBNPlus lbnp(g, base_lb);
    unsigned long lo = lbnp.estimate();
    REQUIRE(lo >= 1);
    REQUIRE(lo <= up);
  }
}
