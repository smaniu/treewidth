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
  boost::unordered_flat_map<unsigned long,
    boost::heap::fibonacci_heap<node_type>::handle_type> queue_nodes;

public:
  //Clears any residual queue state. init_permutation() calls this itself, but
  //it is also exposed for callers that need to reset the queue explicitly.
  void clear(){
    queue.clear();
    queue_nodes.clear();
  }

  //Computes the initial permutation for all nodes in the graph.
  //Clears first: the same strategy instance is reused on fresh/different graphs
  //(e.g. Delta2D per node, and the shared strategy driving both CE::con_edge and
  //a base LowerBound inside LBN/LBN+). A previous call that was not fully drained
  //would otherwise leave stale entries that desync the queue from this graph --
  //get_next() would return a node absent from it (get_neighbours assertion /
  //empty-set fallback).
  void init_permutation(Graph& graph){
    clear();
    for(auto node:graph.get_nodes()){
      node_type nstruct;
      nstruct.id = node;
      nstruct.val = compute_statistic(node, graph, true);
      queue_nodes[node]=queue.push(nstruct);
    }
  }

  unsigned long  emptyQ(){
	  node_type nstruct_temp = queue.top();
	  queue.pop();
	  unsigned long node_id = nstruct_temp.id;	  
	  queue_nodes.erase(node_id);
      return queue.size();
  }
  
  //Recomputes the statistic and updates the queue for a subset of nodes
  virtual void recompute(const boost::unordered_flat_set<unsigned long> &nodes, Graph& graph){
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
  
  //Gets the nodes having the smallest value without deleting
  unsigned long get_next_wo_delete() {
	  node_type nstruct = queue.top();
	  unsigned long node_id = nstruct.id;
	  return node_id;
  }

  //Gets the nodes having the second smallest value and delete it
  unsigned long get_second_next_delete() {
	  node_type nstruct_temp = queue.top();
	  queue.pop();
	  node_type nstruct = queue.top();
	  unsigned long node_id = nstruct.id;
	  queue.pop();
	  queue_nodes.erase(node_id);
	  queue.push(nstruct_temp);
	  return node_id;
  }

  //Gets the nodes having the second smallest value
  unsigned long get_second_next() {
	  node_type nstruct_temp = queue.top();
	  queue.pop();
	  node_type nstruct = queue.top();
	  unsigned long node_id = nstruct.id;
	  queue.push(nstruct_temp);
	  return node_id;
  }
  unsigned long Q_siz(){return queue.size();}
  bool empty_but1() { return !(queue.size()>1); }
  bool empty() {return !(queue.size()>0);}
  
protected:
  //Computes the statistic for a node -- needs to be implemented by subclasses
  virtual unsigned long compute_statistic(unsigned long node, Graph& graph,\
                                          bool initial=false) = 0;
  
};

#endif /* PermutationStrategy_h */
