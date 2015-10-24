/*
 Copyright (c) 2015 Silviu Maniu
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

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
