#ifndef TEST_GRAPH_BUILDERS_H
#define TEST_GRAPH_BUILDERS_H

// Small, deterministic graph constructors and snapshot helpers used by the
// tests. Graphs are built in-process so no fixture files are needed.

#include <algorithm>
#include <set>
#include <utility>

#include "Graph.h"

inline Graph clique(unsigned long n) {
  Graph g;
  for (unsigned long i = 0; i < n; i++)
    for (unsigned long j = i + 1; j < n; j++) g.add_edge(i, j);
  return g;
}

inline Graph path(unsigned long n) {
  Graph g;
  for (unsigned long i = 0; i + 1 < n; i++) g.add_edge(i, i + 1);
  return g;
}

inline Graph cycle(unsigned long n) {
  Graph g = path(n);
  if (n >= 3) g.add_edge(n - 1, 0);
  return g;
}

inline Graph grid(unsigned long rows, unsigned long cols) {
  Graph g;
  auto id = [&](unsigned long r, unsigned long c) { return r * cols + c; };
  for (unsigned long r = 0; r < rows; r++)
    for (unsigned long c = 0; c < cols; c++) {
      if (c + 1 < cols) g.add_edge(id(r, c), id(r, c + 1));
      if (r + 1 < rows) g.add_edge(id(r, c), id(r + 1, c));
    }
  return g;
}

// Three triangles attached via vertex 2 — matches the CLI baseline graph whose
// per-method treewidth values are pinned as goldens.
inline Graph sample_two_triangles() {
  Graph g;
  g.add_edge(0, 1); g.add_edge(1, 2); g.add_edge(2, 3); g.add_edge(3, 0);
  g.add_edge(0, 2); g.add_edge(2, 4); g.add_edge(4, 5); g.add_edge(5, 2);
  return g;
}

// Snapshot the edge set (as ordered pairs) before a destructive decomposition.
inline std::set<std::pair<unsigned long, unsigned long>>
collect_edges(const Graph& g) {
  std::set<std::pair<unsigned long, unsigned long>> edges;
  for (auto u : g.get_nodes())
    if (g.has_neighbours(u))
      for (auto v : g.get_neighbours(u))
        edges.insert({std::min(u, v), std::max(u, v)});
  return edges;
}

inline std::set<unsigned long> collect_vertices(const Graph& g) {
  return std::set<unsigned long>(g.get_nodes().begin(), g.get_nodes().end());
}

#endif  // TEST_GRAPH_BUILDERS_H
