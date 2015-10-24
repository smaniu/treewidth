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

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "common.h"
#include "Graph.h"
#include "PermutationStrategy.h"
#include "DegreePermutationStrategy.h"
#include "FillInPermutationStrategy.h"
#include "TreeDecomposition.h"

void treewidth(int argc, const char * argv[]){
  std::string file_name_graph(argv[2]);
  std::ifstream file(file_name_graph);
  std::stringstream ss, ss_stat;
  timestamp_t t0, t1;
  double time_sec;
  int str = 0;
  int part = 0;
  if(argc>3)
    str = atoi(argv[3]);
  if(argc>4)
    part = atoi(argv[4]);
  ss << file_name_graph << "." << str << "." << part << ".dec";
  ss_stat << file_name_graph << "." << str << "." << part << ".stat";
  Graph graph;
  std::vector<std::unique_ptr<PermutationStrategy>> strategies;
  strategies.push_back(std::unique_ptr<PermutationStrategy>
                       (new DegreePermutationStrategy()));
  strategies.push_back(std::unique_ptr<PermutationStrategy>
                       (new FillInPermutationStrategy()));
  unsigned long src, tgt;
  std::cout << "loading graph..." << std::flush;
  t0 = get_timestamp();
  while(file >> src >> tgt){
    graph.add_edge(src, tgt);
  }
  t1 = get_timestamp();
  time_sec = (t1-t0)/(1000.0L*1000.0L);
  std::cout << " done in " << time_sec << " sec."<< std::endl;
  std::cout << "graph: " << graph.number_nodes() << " nodes " <<\
    graph.number_edges() << " edges" << std::endl;
  TreeDecomposition decomposition(graph, *strategies[str]);
  std::cout << "decomposing..."  << std::flush;
  t0 = get_timestamp();
  decomposition.decompose(part);
  t1 = get_timestamp();
  time_sec = (t1-t0)/(1000.0L*1000.0L);
  std::cout << " done in " << time_sec << " sec."<< std::endl;
  std::cout << "building tree..."  << std::flush;
  t0 = get_timestamp();
  decomposition.build_tree();
  t1 = get_timestamp();
  time_sec = (t1-t0)/(1000.0L*1000.0L);
  std::cout << " done in " << time_sec << " sec."<< std::endl;
  std::cout << "writing decomposition..." << std::flush;
  t0 = get_timestamp();
  std::ofstream filedec(ss.str());
  filedec << decomposition;
  filedec.close();
  std::ofstream filestat(ss_stat.str());
  filestat << decomposition.get_stat();
  filestat.close();
  t1 = get_timestamp();
  time_sec = (t1-t0)/(1000.0L*1000.0L);
  std::cout << " done in " << time_sec << " sec."<< std::endl;

}

int main(int argc, const char * argv[]) {
  std::string first_arg(argv[1]);
  if(first_arg=="--decompose") treewidth(argc, argv);
}
