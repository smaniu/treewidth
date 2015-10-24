#ifndef DegreePermutationStrategy_h
#define DegreePermutationStrategy_h

#include "PermutationStrategy.h"

class DegreePermutationStrategy: public PermutationStrategy{
private:
  unsigned long compute_statistic(unsigned long node, Graph& graph){
    if(graph.has_neighbours(node)){
      return graph.get_neighbours(node).size();
    }
    else{
      return 0;
    }
  }
};


#endif /* DegreePermutationStrategy_h */
