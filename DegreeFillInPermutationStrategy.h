//
//  DegreeFillInPermutationStrategy.h
//  treewidth
//
//  Created by Silviu Maniu on 25/10/15.
//  Copyright © 2015 Silviu Maniu. All rights reserved.
//

#ifndef DegreeFillInPermutationStrategy_h
#define DegreeFillInPermutationStrategy_h

#include "FillInPermutationStrategy.h"

//Implements the degree+fill in strategy for choosing nodes to decompose:
// - at each step, the node with the smallest "fill in" (number of missing edges
//    between negibours)+degree is chosen
class DegreeFillInPermutationStrategy: public FillInPermutationStrategy{
  
protected:
  unsigned long compute_statistic(unsigned long node, Graph& graph,\
                                  bool init=false){
    unsigned long rtval = 0;
    if(graph.has_neighbours(node)) rtval = graph.get_neighbours(node).size();
    return rtval+FillInPermutationStrategy::compute_statistic(node,graph);
  }
};

#endif /* DegreeFillInPermutationStrategy_h */
