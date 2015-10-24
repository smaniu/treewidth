#ifndef PermutationStrategy_h
#define PermutationStrategy_h

#include <boost/heap/fibonacci_heap.hpp>

#include "Graph.h"

//Class for computing the permutation
class PermutationStrategy{
protected:
  struct node_type{
    unsigned long id;
    unsigned long val;
    bool operator<(const node_type &a) const{
      return val>a.val?true:(val<a.val?false:id>a.id);
    }
  };
  boost::heap::fibonacci_heap<node_type> queue;
  std::unordered_map<unsigned long,
    boost::heap::fibonacci_heap<node_type>::handle_type> queue_nodes;

public:
  //Computes the initial permutation for all nodes in the graph
  void init_permutation(Graph& graph){
    for(auto node:graph.get_nodes()){
      node_type nstruct;
      nstruct.id = node;
      nstruct.val = compute_statistic(node, graph);
      queue_nodes[node]=queue.push(nstruct);
    }
  }
  
  //Recomputes the statistic and updates the queue for a subset of nodes
  void recompute(std::unordered_set<unsigned long> nodes, Graph& graph){
    for(auto node:nodes){
      node_type nstruct;
      nstruct.id = node;
      nstruct.val = compute_statistic(node, graph);
      queue.update(queue_nodes[node], nstruct);
    }
  }
  
  //Gets the nodes having the smallest value
  unsigned long get_next(){
    node_type nstruct = queue.top();
    unsigned long node_id = nstruct.id;
    queue.pop();
    queue_nodes.erase(node_id);
    return node_id;
  }
  
  bool empty() {return !(queue.size()>0);}
  
protected:
  //Computes the statistic for a node -- needs to be implemented by subclasses
  virtual unsigned long compute_statistic(unsigned long node, Graph& graph) = 0;
  
};

#endif /* PermutationStrategy_h */
