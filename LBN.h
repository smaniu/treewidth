//
//  LBN.h
//  treewidth
//
//  Created by Silviu Maniu on 12/09/16.
//  Copyright © 2016 Silviu Maniu. All rights reserved.
//

#ifndef LBN_h
#define LBN_h

#include "MetaLowerBoundHeuristic.h"

//Implement the LBN meta-heuristic
class LBN : public MetaLowerBoundHeuristic {
public:
  LBN(Graph& graph, LowerBound& lower_bound) :\
  MetaLowerBoundHeuristic(graph, lower_bound) {};
  
  virtual unsigned long estimate() override {
    Graph graph_temp_estimation = graph;
    lower_bound.setGraph(graph_temp_estimation);
    Graph graph_temp = graph;
    unsigned long lower = lower_bound.estimate();
    bool change = true;
    while(change) {
      //creating the (lower+1)-neighbor improved graph of graph
      change = false;
      for (auto n1:graph.get_nodes()){
        for (auto n2:graph.get_nodes()){
          if (!graph_temp.has_edge(n1,n2)){
            bool retval = graph.neighbour_improved(lower+1,n1,n2);
            if (retval&&n1!=n2) graph_temp.add_edge(n1, n2);
          }
        }
      }
      //if the lower bound on the improved graph is greater
      //increase the lower bound estimation
      lower_bound.setGraph(graph_temp);
      unsigned long lower_temp = lower_bound.estimate();
      graph_temp = graph;
      if (lower_temp > lower){
        lower = lower+1;
        change = true;
      }
    }
    return lower;
  }
};

#endif /* LBN_h */
