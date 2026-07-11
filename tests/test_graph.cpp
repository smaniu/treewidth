#include <unordered_set>

#include "catch.hpp"
#include "Graph.h"
#include "graph_builders.h"

TEST_CASE("add_edge records nodes, edges and undirected adjacency") {
  Graph g;
  g.add_edge(1, 2);
  REQUIRE(g.number_nodes() == 2);
  REQUIRE(g.number_edges() == 1);
  REQUIRE(g.has_edge(1, 2));
  REQUIRE(g.has_edge(2, 1));
  REQUIRE_FALSE(g.has_edge(1, 3));
  REQUIRE(g.has_node(1));
}

TEST_CASE("duplicate edges are ignored") {
  Graph g;
  g.add_edge(1, 2);
  g.add_edge(2, 1);
  g.add_edge(1, 2);
  REQUIRE(g.number_edges() == 1);
}

TEST_CASE("remove_node returns its neighbours and detaches it") {
  Graph g = path(4);  // 0-1-2-3
  auto neigh = g.remove_node(1);
  REQUIRE(neigh.count(0) == 1);
  REQUIRE(neigh.count(2) == 1);
  REQUIRE(g.number_nodes() == 3);
  REQUIRE_FALSE(g.has_node(1));
  REQUIRE_FALSE(g.has_edge(0, 1));
}

TEST_CASE("fill turns a neighbourhood into a clique") {
  Graph g;
  g.add_edge(0, 1);
  g.add_edge(0, 2);
  g.add_edge(0, 3);
  std::unordered_set<unsigned long> ns{1, 2, 3};
  g.fill(ns);
  REQUIRE(g.has_edge(1, 2));
  REQUIRE(g.has_edge(1, 3));
  REQUIRE(g.has_edge(2, 3));
}

TEST_CASE("contract_edge merges the target into the source") {
  Graph g = path(3);      // 0-1-2
  g.contract_edge(0, 1);  // fold 1 into 0
  REQUIRE_FALSE(g.has_node(1));
  REQUIRE(g.has_edge(0, 2));
}
