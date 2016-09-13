//
//  LBNPlus.h
//  treewidth
//
//  Created by Silviu Maniu on 12/09/16.
//  Copyright © 2016 Silviu Maniu. All rights reserved.
//

#ifndef LBNPlus_h
#define LBNPlus_h

#include "MetaLowerBoundHeuristic.h"
#include "CE.h"

//Implement the LBN+ meta-heuristic
class LBNPlus : public MetaLowerBoundHeuristic {
public:
  LBNPlus(Graph& graph, LowerBound& lower_bound) :\
  MetaLowerBoundHeuristic(graph, lower_bound) {};
  
  virtual unsigned long estimate() override {
    Graph graph_temp = graph;
    lower_bound.setGraph(graph_temp);
    unsigned long lower = lower_bound.estimate();
    graph_temp = graph;
    bool change = true;
    CE contract(graph_temp, lower_bound.getStrategy());
    while(change) {
      change = false;
      graph_temp = graph;
      //We need to perform neighbour improvement on graph
      for(auto n1:graph.get_nodes())
        for(auto n2:graph.get_nodes())
          if(!graph_temp.has_edge(n1,n2)) {
            bool retval = graph.neighbour_improved(lower+1,n1,n2);
            if(retval&&n1!=n2) graph_temp.add_edge(n1, n2);
          }
      Graph graph_temp1 = graph_temp;
      lower_bound.setGraph(graph_temp1);
      unsigned long lower_temp = lower_bound.estimate();
      while(lower_temp<=lower&&graph_temp.number_edges()>0) {
        contract.con_edge();
        for(auto n1:graph_temp.get_nodes())
          for (auto n2:graph_temp.get_nodes())
            if(!graph_temp.has_edge(n1,n2)) {
              bool retval = graph_temp.neighbour_improved(lower+1,n1,n2);
              if(retval&&n1!=n2) graph_temp.add_edge(n1, n2);
            }
        graph_temp1 = graph_temp;
        lower_bound.setGraph(graph_temp1);
        lower_temp = lower_bound.estimate();
      }
      if(lower_temp>lower) {
        lower = lower+1;
        change = true;
      }
    }
    return lower;
  }
};

#endif /* LBNPlus_h */
