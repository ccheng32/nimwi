#include "C_graph.h"
#include "C_graph_algo.h"

double C_graph::SIM(int k, char *output_file) {
  int i, j;
  int v_m, v_a;
  int current_max;
  FILE *fp;

  fp = fopen(output_file, "w");

  for (i = 1; i <= k; i++) {
    current_max = 0;
    for (j = 0; j < max_node; j++) {
      if (nodes[j].is_existed && nodes[j].lcc > current_max) {
        current_max = nodes[j].lcc;
        v_m = j;
      }
    }

    // printf("v_m %d\n", v_m);

    // find the farthest node
    v_a = returnFarthestNodeFrom(v_m);

    // printf("v_a %d\n", v_a);

    // printf("\t %d %d \n", v_m, v_a);
    fprintf(fp, "%d %d\n", v_m, v_a);

    updateForAnNewEdge(v_m, v_a);
    addEdge(v_m, v_a);

    if (i % 100 == 0) {
      calculateAllNodeLCC();
    } else if (i % 1500 == 0) {
      calculateAllNodeLCC();
    }
  }

  fclose(fp);
  // calculateAllNodeLCC();
  returnMaxLCC();
  return max_lcc;
}
