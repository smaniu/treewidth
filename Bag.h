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
  Bag(unsigned long id, const std::unordered_set<unsigned long> &nodeset){
    this->id = id;
    nodes.reserve(nodeset.size());
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
  out << bag.id << "\n";
  out << bag.nodes.size() << "\n";
  for(auto node:bag.nodes) out << node << "\t";
  out << "\n";
  out << bag.parent << "\n";
  out << bag.children.size() << "\n";
  for(auto node:bag.children) out << node << "\t";
  if(bag.children.size()>0) out << "\n";
  return out;
}

#endif /* Bag_h */
