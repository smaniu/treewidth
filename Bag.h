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

#ifndef Bag_h
#define Bag_h

#include <unordered_set>
#include <vector>
#include <ostream>

//A node in the tree deocomposition
class Bag{
private:
  unsigned long id;
  std::vector<unsigned long> nodes;
  unsigned long parent=0;
  std::vector<unsigned long> children;
public:
  Bag(unsigned long id, std::unordered_set<unsigned long> nodeset){
    this->id = id;
    for(unsigned long node:nodeset) nodes.push_back(node);
  }
  
  void set_parent(unsigned long parent){
    this->parent = parent;
  }
  
  void add_to_children(unsigned long node){
    children.push_back(node);
  }
  
  std::vector<unsigned long>& get_nodes(){
    return nodes;
  }
  
  friend std::ostream& operator<<(std::ostream& out, Bag& bag);
  
};

std::ostream& operator<<(std::ostream& out, Bag& bag){
  out << bag.id << std::endl;
  out << bag.nodes.size() << std::endl;
  for(auto node:bag.nodes) out << node << "\t";
  out << std::endl;
  out << bag.parent << std::endl;
  out << bag.children.size() << std::endl;
  for(auto node:bag.children) out << node << "\t";
  if(bag.children.size()>0) out << std::endl;
  return out;
}

#endif /* Bag_h */
