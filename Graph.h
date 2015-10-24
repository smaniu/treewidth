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

#ifndef Graph_h
#define Graph_h

#include <unordered_map>
#include <unordered_set>

//Class for the graph
class Graph{
private:
  std::unordered_map<unsigned long, std::unordered_set<unsigned long>> adj_list;
  std::unordered_set<unsigned long> node_set;
  unsigned long num_edges = 0;
  
public:
  void add_edge(unsigned long src, unsigned long tgt, bool undirected=true){
    node_set.insert(src);
    node_set.insert(tgt);
    if(!has_edge(src, tgt)){
      adj_list[src].insert(tgt);
      if(undirected) adj_list[tgt].insert(src);
      num_edges++;
    }
  };
  
  void add_node(unsigned long node){
    node_set.insert(node);
  }
  
  void remove_node(unsigned long node){
    node_set.erase(node);
    for(auto neighbour:adj_list[node]){
      adj_list[neighbour].erase(node);
      num_edges--;
    }
    adj_list.erase(node);
  }
  
  void fill(const std::unordered_set<unsigned long>& nodes,\
            bool undirected=true){
    for(auto src: nodes)
      for(auto tgt: nodes)
        if(undirected){
          if((src<tgt)&&(adj_list[src].find(tgt)==adj_list[src].end()))
            add_edge(src, tgt, undirected);
        }
        else{
          if((src!=tgt)&&(adj_list[src].find(tgt)==adj_list[src].end()))
            add_edge(src, tgt, undirected);
        }
    
  }
  
  bool has_neighbours(unsigned long node) const{
    return adj_list.find(node)!=adj_list.end();
  }
  
  bool has_node(unsigned long node) const{
    return node_set.find(node)!=node_set.end();
  }
  
  bool has_edge(unsigned long src, unsigned long tgt) {
    bool retval = false;
    if(has_neighbours(src)){
      std::unordered_set<unsigned long> neigh = get_neighbours(src);
      retval = neigh.find(tgt)!=neigh.end();
    }
    return retval;
  }
  
  std::unordered_set<unsigned long> get_neighbours(unsigned long node) const{
    if(has_node(node))
      return (adj_list.find(node))->second;
    else
      return std::unordered_set<unsigned long>();
  }
  
  std::unordered_set<unsigned long> get_nodes() const{
    return node_set;
  }
  
  unsigned long number_nodes() const{
    return node_set.size();
  }
  
  unsigned long number_edges() const{
    return num_edges;
  }
};


#endif /* Graph_h */
