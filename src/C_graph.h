#ifndef C_GRAPH
#define C_GRAPH

#include "C_node.h"

class C_graph {
 public:
  int max_node;
  double max_lcc;

  C_node *nodes;
  // read input from files
  void inputGraph(char file_name[]);
  void inputLCC(char file_name[]);

  // constructor destructors
  C_graph();
  ~C_graph();
  void addEdge(int id_1, int id_2);

  // return value is the number of common neighbors, neighbors are stored in
  // common_neighbors
  int returnCommonNeighbors(int u, int v, int *common_neighbors);

  double calculateOneNodeLCC(int id);

  void calculateAllNodeLCC();

  double returnMaxLCC();

  // call after the lcc has been calculated
  void calculateUpperBound(int k);

  // obtain update after add edge (u, v)
  // call before the edge is added into the graph
  void updateForAnNewEdge(int u, int v);

  // with acceleration
  void updateForAnNewEdgeA(int u, int v, int k);

  int returnFarthestNodeFrom(int id);

  int returnOptionality(int id, int *optional);

  int returnOptionality(int id);

  // NIMWI algorithms
  double OISA(int k, char *output_file);
  double SIM(int k, char *output_file);
  double PONF(int k, char *output_file);
  double BUM(int k, char *output_file);

  void test();
};

#endif
