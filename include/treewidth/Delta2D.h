//
//  Delta2D.h
//  treewidth
//
//  Created by Silviu Maniu on 23/02/16.
//  Copyright � 2016 Silviu Maniu. All rights reserved.
//

#ifndef Delta2D_h
#define Delta2D_h

#include <ostream>
#include <string>
#include <algorithm>
#include <stdlib.h> 

#include "PermutationStrategy.h"
#include "Graph.h"
#include "LowerBound.h"

class Delta2D : public LowerBound{
private:
  Graph graph_temp;
  
public:
	Delta2D(Graph& gr, PermutationStrategy& str) :\
		LowerBound(gr, str) {}

	//Computes estimation
  virtual unsigned long estimate(unsigned long prevBound=0) override {
    unsigned long treewidth = prevBound;
    for(auto node:graph.get_nodes()) treewidth=estimateForNode(node,treewidth);
    return treewidth;
  }
  
private:
  unsigned long estimateForNode(unsigned long prevNode,\
                                unsigned long prevBound){
		//building the first permutation
    unsigned long treewidth = prevBound;
		graph_temp = graph;
		//The loop below intentionally stops once only one node remains in the
		//queue (empty_but1()), so the queue is never left empty after a call.
		//Since `strategy` is shared across every call to estimateForNode
		//(Delta2D::estimate() reuses it for each node in the graph), it must be
		//cleared here or the leftover entry from the previous call would desync
		//the queue from this call's freshly-copied graph_temp.
		strategy.clear();
		strategy.init_permutation(graph_temp);
		while (!strategy.empty_but1()) {
			unsigned long node = strategy.get_second_next();
			const boost::unordered_flat_set<unsigned long>& neigh = graph_temp.get_neighbours(node);
			treewidth = std::max(treewidth, neigh.size());
			unsigned long node_temp = strategy.get_next_wo_delete();
			if (node_temp != prevNode ) {
				node_temp = strategy.get_next();
      }
      else {
				node_temp = strategy.get_second_next_delete();
			}
      graph_temp.remove_node(node_temp);
		}
		return treewidth;
}
};


#endif /* Delta2D_h */
