#ifndef TEST_DECOMPOSITION_CHECK_H
#define TEST_DECOMPOSITION_CHECK_H

// Parses the serialized `.dec` output and validates the three tree-decomposition
// properties: vertex coverage, edge coverage, and the running-intersection
// property (bags containing a given vertex form a connected subtree).

#include <algorithm>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

struct ParsedBag {
  unsigned long id;
  std::vector<unsigned long> nodes;
  unsigned long parent;
  std::vector<unsigned long> children;
};

struct ParsedDecomp {
  unsigned long treewidth = 0;
  std::vector<ParsedBag> bags;
};

// Bags are serialized in id order (position == id); parent ids index into bags.
inline ParsedDecomp parse_decomposition(const std::string& text) {
  std::istringstream in(text);
  ParsedDecomp d;
  unsigned long num_bags = 0;
  in >> d.treewidth >> num_bags;
  d.bags.resize(num_bags);
  for (unsigned long b = 0; b < num_bags; b++) {
    ParsedBag bag;
    unsigned long num_nodes = 0;
    in >> bag.id >> num_nodes;
    bag.nodes.resize(num_nodes);
    for (unsigned long i = 0; i < num_nodes; i++) in >> bag.nodes[i];
    unsigned long num_children = 0;
    in >> bag.parent >> num_children;
    bag.children.resize(num_children);
    for (unsigned long i = 0; i < num_children; i++) in >> bag.children[i];
    d.bags[b] = bag;
  }
  return d;
}

inline bool covers_all_vertices(const ParsedDecomp& d,
                                const std::set<unsigned long>& verts) {
  std::set<unsigned long> seen;
  for (const auto& bag : d.bags)
    for (auto n : bag.nodes) seen.insert(n);
  for (auto v : verts)
    if (!seen.count(v)) return false;
  return true;
}

inline bool covers_all_edges(
    const ParsedDecomp& d,
    const std::set<std::pair<unsigned long, unsigned long>>& edges) {
  for (const auto& e : edges) {
    bool found = false;
    for (const auto& bag : d.bags) {
      std::set<unsigned long> ns(bag.nodes.begin(), bag.nodes.end());
      if (ns.count(e.first) && ns.count(e.second)) {
        found = true;
        break;
      }
    }
    if (!found) return false;
  }
  return true;
}

inline bool running_intersection(const ParsedDecomp& d) {
  const size_t k = d.bags.size();
  std::vector<std::vector<size_t>> adj(k);
  for (size_t i = 0; i < k; i++) {
    size_t p = d.bags[i].parent;
    if (p != i && p < k) {
      adj[i].push_back(p);
      adj[p].push_back(i);
    }
  }
  std::set<unsigned long> verts;
  for (const auto& bag : d.bags)
    for (auto n : bag.nodes) verts.insert(n);
  for (auto v : verts) {
    std::set<size_t> inset;
    for (size_t i = 0; i < k; i++) {
      const auto& nodes = d.bags[i].nodes;
      if (std::find(nodes.begin(), nodes.end(), v) != nodes.end())
        inset.insert(i);
    }
    if (inset.size() <= 1) continue;
    std::set<size_t> visited;
    std::queue<size_t> q;
    size_t start = *inset.begin();
    q.push(start);
    visited.insert(start);
    while (!q.empty()) {
      size_t cur = q.front();
      q.pop();
      for (size_t nb : adj[cur])
        if (inset.count(nb) && !visited.count(nb)) {
          visited.insert(nb);
          q.push(nb);
        }
    }
    if (visited.size() != inset.size()) return false;
  }
  return true;
}

inline bool is_valid_tree_decomposition(
    const ParsedDecomp& d,
    const std::set<std::pair<unsigned long, unsigned long>>& edges,
    const std::set<unsigned long>& verts) {
  return covers_all_vertices(d, verts) && covers_all_edges(d, edges) &&
         running_intersection(d);
}

#endif  // TEST_DECOMPOSITION_CHECK_H
