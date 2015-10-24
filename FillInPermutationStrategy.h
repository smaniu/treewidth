#ifndef FillInPermutationStrategy_h
#define FillInPermutationStrategy_h

#include <unordered_set>

#include "PermutationStrategy.h"

//Implements the fill in strategy for choosing nodes to decompose:
// - at each step, the node with the smallest "fill in" (number of missing edges
//    between negibours) is chosen
class FillInPermutationStrategy: public PermutationStrategy{
public:
  //For fill in, we need to look also at neighbours of neighbours
  void recompute(std::unordered_set<unsigned long> nodes, Graph& graph){
    std::unordered_set<unsigned long> new_nodes;
    for(auto node:nodes){
      new_nodes.insert(node);
      if(graph.has_neighbours(node))
        for(auto nnode:graph.get_neighbours(node)) new_nodes.insert(nnode);
    }
    for(auto node:new_nodes){
      node_type nstruct;
      nstruct.id = node;
      nstruct.val = compute_statistic(node, graph);
      queue.update(queue_nodes[node], nstruct);
    }
  }
  
private:
  //Computes fill in for a node
  unsigned long compute_statistic(unsigned long node, Graph& graph){
    unsigned long fillin = 0;
    if(graph.has_neighbours(node)){
      std::unordered_set<unsigned long> nodes = graph.get_neighbours(node);
      for(auto src: nodes){
        if(graph.has_neighbours(src)){
          std::unordered_set<unsigned long> neigh = graph.get_neighbours(src);
          for(auto tgt: nodes)
            if((src<tgt)&&(neigh.find(tgt)==neigh.end())) fillin++;
        }
      }
    }
    return fillin;
  }
};

#endif /* FillInPermutationStrategy_h */
