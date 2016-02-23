#ifndef MCSPermutationStrategy_h
#define MCSPermutationStrategy_h

#include <unordered_set>

#include "PermutationStrategy.h"

//Implements the Maximum Cardinality Search permutation stategy
// - at each step, the node with the largest number of already visited nodes
//    is chosen
class MCSPermutationStrategy: public PermutationStrategy{
public:
  void recompute(std::unordered_set<unsigned long> nodes, Graph& graph){
    for(auto node:graph.get_nodes()){
      node_type nstruct;
      nstruct.id = node;
      nstruct.val = (*queue_nodes[node]).val-1;
      queue_nodes[node]=queue.push(nstruct);
    }
  }
  
protected:
  //This is only useful for the initial phase, for initializing the heap
  unsigned long compute_statistic(unsigned long node, Graph& graph,\
                                  bool init=false){
    unsigned long rtval = 0;
    if(init) rtval = graph.number_nodes();
    return rtval;
  }
};

#endif /* FillInPermutationStrategy_h */