//
//  CE.h
//  treewidth
//
//  Created by Silviu Maniu on 23/02/16.
//  Copyright © 2016 Silviu Maniu. All rights reserved.
//

#ifndef CE_h
#define CE_h

#include <ostream>
#include <string>
#include <algorithm>

#include "PermutationStrategy.h"
#include "Graph.h"

//Class implementing a greedy estimation of the lower bound for graph
// degeneracy, which is a lower bound on the treewidth of the graph
class CE{
private:
  Graph& graph;
  PermutationStrategy& strategy;
public:
  CE(Graph& gr, PermutationStrategy& str):\
  graph(gr), strategy(str) {}  
  //Computes estimation
  
  void con_edge(){
    //building the first permutation
    std::cout << "graph: " << graph.number_nodes() << " nodes " <<\
    graph.number_edges() << " edges" << std::endl;
    strategy.init_permutation(graph);
    unsigned long node = strategy.get_next();
    std::unordered_set<unsigned long> neigh = graph.get_neighbours(node);
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
      //contracting the edge
      graph.contract_edge(min_v,node);
    }
  
}
};

#endif /* CE_h */
