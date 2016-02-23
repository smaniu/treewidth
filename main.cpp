#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "common.h"
#include "Graph.h"
#include "PermutationStrategy.h"
#include "DegreePermutationStrategy.h"
#include "FillInPermutationStrategy.h"
#include "DegreeFillInPermutationStrategy.h"
#include "MCSPermutationStrategy.h"
#include "TreeDecomposition.h"

void upper_bound(int argc, const char * argv[]){
  std::string file_name_graph(argv[2]);
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
  strategies.push_back(std::unique_ptr<PermutationStrategy>
                       (new DegreeFillInPermutationStrategy()));
  strategies.push_back(std::unique_ptr<PermutationStrategy>
                       (new MCSPermutationStrategy()));
  unsigned long src, tgt;
  std::cout << "loading graph..." << std::flush;
  t0 = get_timestamp();
  std::ifstream file(file_name_graph);
  while(file >> src >> tgt){
    if(src!=tgt) graph.add_edge(src, tgt);
  }
  file.close();
  std::ofstream filestat(ss_stat.str());
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
  filestat << time_sec << std::endl;
  std::cout << "building tree..."  << std::flush;
  t0 = get_timestamp();
  decomposition.build_tree();
  t1 = get_timestamp();
  time_sec = (t1-t0)/(1000.0L*1000.0L);
  std::cout << " done in " << time_sec << " sec."<< std::endl;
  filestat << time_sec << std::endl;
  std::cout << "writing decomposition..." << std::flush;
  t0 = get_timestamp();
  std::ofstream filedec(ss.str());
  filedec << decomposition;
  filedec.close();
  filestat << decomposition.get_stat();
  filestat.close();
  t1 = get_timestamp();
  time_sec = (t1-t0)/(1000.0L*1000.0L);
  std::cout << " done in " << time_sec << " sec."<< std::endl;

}

int main(int argc, const char * argv[]) {
  std::string first_arg(argv[1]);
  if(first_arg=="--upper") upper_bound(argc, argv);
}
