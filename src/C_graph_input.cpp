#include "C_graph.h"
#include <cstdio>
#include <cstdlib>
#include "C_graph_algo.h"

void C_graph::inputGraph(char file_name[]) {
  int id_1, id_2;
  FILE *fp;
  fp = fopen(file_name, "r");

  while (fscanf(fp, "%d%d", &id_1, &id_2) == 2) {
    // printf("%d %d\n", id_1, id_2);
    addEdge(id_1, id_2);
  }

  fclose(fp);

  for (int i = 0; i <= max_node; i++) {
    if (nodes[i].is_existed) {
      qsort(nodes[i].neighbors, nodes[i].degree, sizeof(int), cmp);
    }
  }

  for (int i = 0; i < num_nodes; i++) {
    printf("%d:", i);
    if (nodes[i].is_existed) {
      for (int j = 0; j < nodes[i].degree; j++) {
        printf(" %d", nodes[i].neighbors[j]);
      }
    }
    printf("\n");
  }

  return;
}

void C_graph::inputLCC(char file_name[]) {
  int id;
  double lcc;

  FILE *fp;
  fp = fopen(file_name, "r");

  while (fscanf(fp, "%d%lf", &id, &lcc) == 2) {
    nodes[id].lcc = lcc;
    // printf("%ld %.2lf\n",id, lcc);
  }

  fclose(fp);

  return;
}
