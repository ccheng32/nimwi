#include <cstdio>
#include <cstdlib>

#include "C_graph.h"
#include "C_graph_algo.h"

void C_graph::inputGraph(char file_name[]) {
  int id_1, id_2;
  FILE* fp;
  fp = fopen(file_name, "r");

  while (fscanf(fp, "%d%d", &id_1, &id_2) == 2) {
    // printf("%d %d\n", id_1, id_2);
    addEdge(id_1, id_2);
  }

  fclose(fp);

  // initalize LCC values
  calculateAllNodeLCC();

  return;
}
