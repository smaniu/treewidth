//
//  Delta2D.h
//  treewidth
//
//  Created by Silviu Maniu on 23/02/16.
//  Copyright © 2016 Silviu Maniu. All rights reserved.
//

#ifndef Delta2D_h
#define Delta2D_h

#include <ostream>
#include <string>
#include <algorithm>
#include <stdlib.h> 

#include "PermutationStrategy.h"
#include "Graph.h"


class Delta2D {
private:

public:
	Graph& graph;
	Graph& graph_temp;
	PermutationStrategy& strategy;

	Delta2D(Graph& gr, Graph& gr_temp, PermutationStrategy& str) :\
		graph(gr), graph_temp(gr_temp), strategy(str) {}

	//Computes estimation
	unsigned long estimate(unsigned long n, unsigned long treewidth) {
		//building the first permutation
		graph_temp = graph;
		strategy.init_permutation(graph_temp);
		while (!strategy.empty_but1()) {
			unsigned long node = strategy.get_second_next();
			std::unordered_set<unsigned long> neigh = graph_temp.get_neighbours(node);
			treewidth = std::max(treewidth, neigh.size());
			unsigned long node_temp = strategy.get_next_wo_delete();
			if (node_temp != n ) {
				node_temp = strategy.get_next();
			}
			if (node_temp == n) {
				node_temp = strategy.get_second_next_delete();
			}
			
			graph_temp.remove_node(node_temp);

		}
		
		return treewidth;
}
};


#endif /* Delta2D_h */
