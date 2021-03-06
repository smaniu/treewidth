#ifndef DegreePermutationStrategy_h
#define DegreePermutationStrategy_h

#include "PermutationStrategy.h"

class DegreePermutationStrategy: public PermutationStrategy{
protected:
  unsigned long compute_statistic(unsigned long node, Graph& graph,\
                                  bool =false){
    if(graph.has_neighbours(node)){
      return graph.get_neighbours(node).size();
    }
    else{
      return 0;
    }
  }
};


#endif /* DegreePermutationStrategy_h */
