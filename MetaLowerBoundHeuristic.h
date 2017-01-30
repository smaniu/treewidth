//
//  MetaLowerBoundHeuristic.h
//  treewidth
//
//  Created by Silviu Maniu on 12/09/16.
//  Copyright © 2016 Silviu Maniu. All rights reserved.
//

#ifndef MetaLowerBoundHeuristic_h
#define MetaLowerBoundHeuristic_h

#include "Graph.h"
#include "LowerBound.h"

class MetaLowerBoundHeuristic {
protected:
  Graph& graph;
  LowerBound& lower_bound;
public:
  MetaLowerBoundHeuristic(Graph& graph, LowerBound& lower_bound) :\
  graph(graph), lower_bound(lower_bound) {};
  
  virtual unsigned long estimate()=0;
};


#endif /* MetaLowerBoundHeuristic_h */
