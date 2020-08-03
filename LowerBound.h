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

class LowerBound {
protected:
  Graph& graph;
  PermutationStrategy& strategy;
public:
  LowerBound(Graph& graph, PermutationStrategy& strategy) :\
  graph(graph), strategy(strategy) {};
  
  void setGraph(Graph& newGraph) { graph = newGraph;}
  
  void setStrategy(PermutationStrategy& newStrategy) {strategy = newStrategy;}
  
  Graph& getGraph() {return graph;}
  
  PermutationStrategy& getStrategy() {return strategy;}
  
  virtual unsigned long estimate(unsigned long prevBound=0)=0;
};

//Class implementing a greedy estimation of the lower bound for graph
// degeneracy, which is a lower bound on the treewidth of the graph
class LowerBoundMMDPlus: public LowerBound{
private:
  unsigned long treewidth = 0;
public:
  LowerBoundMMDPlus(Graph& gr, PermutationStrategy& str):\
  LowerBound(gr, str) {};
  
  //Computes estimation
  virtual unsigned long estimate(unsigned long =0) override{
    //building the first permutation
    //std::cout << "graph: " << graph.number_nodes() << " nodes " <<\
    //graph.number_edges() << " edges" << std::endl;
    strategy.init_permutation(graph);
    treewidth = 0;
    unsigned long num_node = 0;
    //looping greedily through the permutation

    unsigned long u = 0;

    while(!strategy.empty()){
      unsigned long node = strategy.get_next();
      u = u+1;
      std::unordered_set<unsigned long> neigh = graph.get_neighbours(node);
      treewidth = std::max(treewidth,neigh.size());
      //getting the neighbour with least overlap
      unsigned long min_v = 0;
      unsigned long min_ovl = neigh.size();
      //std::cout<<"2 point\n";
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
      //std::cout<<u<<"\n";
    }
    //std::cout<<"end point\n";
    return treewidth;
  }
};


#endif /* LowerBound_h */
