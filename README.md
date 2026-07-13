[![build](https://github.com/smaniu/treewidth/actions/workflows/ci.yml/badge.svg)](https://github.com/smaniu/treewidth/actions/workflows/ci.yml)
[![tests](https://img.shields.io/github/actions/workflow/status/smaniu/treewidth/ci.yml?label=tests)](https://github.com/smaniu/treewidth/actions/workflows/ci.yml)
[![MIT licensed](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/smaniu/treewidth/master/LICENSE)

This repository contains the source code for evaluation of lower and upper bounds of the
treewidth of undirected graphs. It implements a subset of the algorithms
described in [this paper][1] and [this paper][2]. 

Experimental results were published in our ICDT 2019 paper [An Experimental Study of the Treewidth of Real-World Graph Data][3].

# Compiling

The project builds with CMake (3.20+):

    cmake -B build
    cmake --build build

The output binary is *build/treedecomp*.

## Dependencies

The code needs the Boost C++ library headers, and specifically the Fibonacci
heap implementation (no Boost library is linked). CMake locates them via
`find_package(Boost)`; on Debian/Ubuntu install them with `apt-get install
libboost-dev`; on macOS with `brew install boost`.

# Testing

The test suite (using Catch2, in `tests/`) covers graph operations, the
upper/lower bound heuristics, and decomposition verification. 

It is built by default alongside `treedecomp` and run via CTest:

    cmake -B build
    cmake --build build
    ctest --test-dir build --output-on-failure

# Usage

The program expects as input a graph in the following format, one edge per line:
    
    node1 <TAB> node2

where *node1* and *node2* are the endpoints of the edge.

The usage is the following:

    ./treedecomp --upper|lower <graph_file> [<method> [<partial_degree>]]

The parameters are:

* *upper|lower* chooses between upper or lower bounds
* *graph_file* is the name of the graph file
* *method* is the criteria by which each node is chosen next in the
  decomposition sequence, and can have the following values: **0** minimum
  degree (default), **1** minimum fill in, and **2** minimum degree+fill in
  (only for upper bounds)
* *partial_degree* is the maximum degree at which the decomposition will stop;
  in this case, the output files will contain a partial decomposition -- this is
  useful for large graphs (only for upper bounds)
 
## Output

The output will be a file with the name *<graph>.<method>.dec* containing the
tree decomposition.

The format is the following:
    
    <treewidth>
    <number_nodes>
    <node_list>

*treewidth* is a number representing the estimated treewidth, *number_nodes* is
the number of nodes in the tree decomposition of the graph.

*<node_list* contains *number_nodes* blocks representing the tree nodes, each in
the following format:
    
    <node_id>
    <num_nodes>
    <node_list>
    <parent_id>
    <num_children>
    [<child_list>]

*node_id* is the id of the nodes, *num_nodes* is the number of graph nodes in
the tree node, *node_list* is a whitespace delimited list of the nodes,
*parent_id* is the id of the parent of the node in the tree, *num_children* is
the number of child nodes in the tree, and *child_list* is the list of their ids
(if any).

# Datasets

This code has been tested with the graph datasets available [here][6]

# Contributors

* [Silviu Maniu][4] -- upper, lower bound algorithm
* [Suraj Sanjay Jog][5] -- meta-algorithms for lower bounds

# License

The source code is provided as-is under an MIT License.
  
[1]: <http://www.math2.rwth-aachen.de/~koster/paper/boko09a.pdf> "H. L. Bodlaender, A. M. C. A. Koster. Treewidth Computations I. Upper Bounds. Information and Computation 208(3) 2010"
[2]: <http://www.cs.uu.nl/research/techreps/repo/CS-2010/2010-022.pdf> "H. L. Bodlaender, A. M. C. A. Koster. Treewidth Computations II. Lower Bounds. Information and Computation 209(7) 2011"
[3]: <https://arxiv.org/pdf/1901.06862> "S. Maniu, P. Senellart, S. Jog. An Experimental Study of the Treewidth of Real-World Graph Data. ICDT 2019"
[4]: <https://silviu.maniu.info/>
[5]: <https://scholar.google.com/citations?user=N0Lv5FUAAAAJ&hl=en>
[6]: <https://zenodo.org/records/15068283>
