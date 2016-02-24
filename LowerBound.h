//
//  LowerBound.h
//  treewidth
//
//  Created by Silviu Maniu on 23/02/16.
//  Copyright © 2016 Silviu Maniu. All rights reserved.
//

#ifndef LowerBound_h
#define LowerBound_h

#include <ostream>
#include <string>
#include <algorithm>

#include "PermutationStrategy.h"
#include "Graph.h"

//Class implementing a greedy estimation of the lower bound for graph
// degeneracy, which is a lower bound on the treewidth of the graph
class LowerBound{
private:
  Graph& graph;
  PermutationStrategy& strategy;
  unsigned long treewidth = 0;
public:
  LowerBound(Graph& gr, PermutationStrategy& str):\
  graph(gr), strategy(str) {}
  
  //Computes estimation
  unsigned long estimate(unsigned long partial_degree=0){
    //building the first permutation
    strategy.init_permutation(graph);
    double original_nodes = graph.number_nodes();
    unsigned long one = std::max((unsigned long)1,\
                                 (unsigned long)(graph.number_nodes()/100));
    std::string progress= "0% ";
    std::cout << progress << std::flush;
    treewidth = 0;
    unsigned long num_node = 0;
    //looping greedily through the permutation
    while(!strategy.empty()){
      //getting the next node
      unsigned long node = strategy.get_next();
      std::unordered_set<unsigned long> neigh = graph.get_neighbours(node);
      treewidth = std::max(treewidth,neigh.size());
      //getting the neighbour with least overlap
      unsigned long min_v = 0;
      unsigned long min_ovl = neigh.size();
      for(auto v:neigh){
        unsigned long ovl = 0;
        for(auto nv:graph.get_neighbours(v))
          if(graph.has_edge(node,nv)) ovl++;
        if(min_ovl>=ovl){
          min_v = v;
          min_ovl = ovl;
        }
      }
      //contracting the edge
      graph.contract_edge(min_v,node);
      //recomputing the degrees in the graph
      strategy.recompute(neigh, graph);
      num_node++;
      if(num_node%one==0){
        std::cout << std::string(progress.length(),'\b') << std::flush;
        std::stringstream ss_progress;
        ss_progress << (unsigned long)((num_node*100)/original_nodes) << "% ";
        progress = ss_progress.str();
        std::cout << progress << std::flush;
      }
    }
    std::cout << std::string(progress.length(),'\b') << std::flush;
    std::cout << "100% " << std::flush;
    return treewidth;
  }
};


#endif /* LowerBound_h */
