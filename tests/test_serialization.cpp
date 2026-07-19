#include <iostream>
#include <sstream>
#include <boost/unordered/unordered_flat_set.hpp>

#include "catch.hpp"
#include "Bag.h"
#include "Graph.h"
#include "DegreePermutationStrategy.h"
#include "TreeDecomposition.h"
#include "graph_builders.h"

TEST_CASE("Bag serializes id, node list, parent and child count") {
  boost::unordered_flat_set<unsigned long> ns{7};
  Bag bag(3, ns);
  std::ostringstream ss;
  ss << bag;
  std::istringstream in(ss.str());
  unsigned long id, count, node, parent, num_children;
  in >> id >> count >> node >> parent >> num_children;
  REQUIRE(id == 3);
  REQUIRE(count == 1);
  REQUIRE(node == 7);
  REQUIRE(num_children == 0);
}

TEST_CASE("decomposition header reports treewidth then bag count") {
  Graph g = clique(4);
  DegreePermutationStrategy s;
  TreeDecomposition dec(g, s);
  unsigned long tw = dec.decompose();
  dec.build_tree();
  std::ostringstream ss;
  ss << dec;
  std::istringstream in(ss.str());
  unsigned long first_tw, num_bags;
  in >> first_tw >> num_bags;
  REQUIRE(first_tw == tw);
  REQUIRE(first_tw == 3);  // clique(4) has treewidth 3
  REQUIRE(num_bags >= 1);
}
