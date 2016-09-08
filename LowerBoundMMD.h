//
//  LowerBoundMMD.h
//  treewidth
//
//  Created by Silviu Maniu on 23/02/16.
//  Copyright � 2016 Silviu Maniu. All rights reserved.
//

#ifndef LowerBoundMMD_h
#define LowerBoundMMD_h

#include <ostream>
#include <string>
#include <algorithm>

#include "PermutationStrategy.h"
#include "Graph.h"
#include "LowerBound.h"

//Class implementing an estimation of the minimum maximum degree in the graphs,
//which is a lower bound on graph treewidth
class LowerBoundMMD : public LowerBound {
private:
	Graph& graph;
	PermutationStrategy& strategy;
	unsigned long treewidth = 0;
public:
	LowerBoundMMD(Graph& gr, PermutationStrategy& str) :\
		graph(gr), strategy(str) {}

	//Computes estimation
  virtual unsigned long estimate(unsigned long prevBound=0) override {
		std::cout << "graph: " << graph.number_nodes() << " nodes " <<\
  		graph.number_edges() << " edges" << std::endl;
  
		//building the first permutation
		strategy.init_permutation(graph);
		treewidth = 0;
		unsigned long num_node = 0;
		//looping greedily through the permutation
		unsigned long u = 0;
		while (!strategy.empty()) {
			//getting the next node
			unsigned long node = strategy.get_next();
			u = u+1;
			std::cout<<"Point 1\n";
			std::unordered_set<unsigned long> neigh = graph.get_neighbours(node);
			treewidth = std::max(treewidth, neigh.size());
			std::cout<<"Point 2\n";
		//Remove the Node
		graph.remove_node(node);
		std::cout<<"The next line is the error for sure\n";
		//recomputing the degrees in the graph
		strategy.recompute(neigh, graph);
		std::cout<<"The above lines are not the error\n";
		num_node++;
		std::cout<<u<<"\n";
	}
	return treewidth;
}
};


#endif /* LowerBoundMMD_h */
