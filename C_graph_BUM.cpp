#include "C_graph.h"
#include "C_graph_algo.h"

double C_graph::BUM(int k, char *output_file) {
  int i, j;
  int v_m, v_a;
  int current_max;
  FILE *fp;

  clock_t t3, t2;

  fp = fopen(output_file, "w");

  t2 = clock();

  for (i = 1; i <= k; i++) {
    current_max = 0;
    for (j = 0; j < max_node; j++) {
      if (nodes[j].is_existed && nodes[j].lcc > current_max) {
        current_max = nodes[j].lcc;
        v_m = j;
      }
    }

    current_max = 0;
    for (j = 0; j < max_node; j++) {
      if (nodes[j].is_existed && j != v_m && !nodes[v_m].hasNeighbor(j) &&
          nodes[j].lcc > current_max) {
        // printf("* %d %d %d\n", v_m, j, nodes[v_m].hasNeighbor(j));
        current_max = nodes[j].lcc;
        v_a = j;
      }
    }

    // printf("\t %d %d \n", v_m, v_a);
    fprintf(fp, "%d %d\n", v_m, v_a);
    updateForAnNewEdge(v_m, v_a);
    addEdge(v_m, v_a);
    // test();
    qsort(nodes[v_m].neighbors, nodes[v_m].degree, sizeof(int), cmp);
    qsort(nodes[v_a].neighbors, nodes[v_a].degree, sizeof(int), cmp);

    if (i % 100 == 0) {
      calculateAllNodeLCC();
    } else if (i % 1500 == 0) {
      t3 = clock();
      calculateAllNodeLCC();
      printf("BUM\t%d\t%.2lf\t%.2lf\n", i, returnMaxLCC(),
             double(t3 - t2) / CLOCKS_PER_SEC);
    }
  }

  fclose(fp);
  returnMaxLCC();
  return max_lcc;
}
