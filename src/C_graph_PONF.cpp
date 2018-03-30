#include "C_graph.h"
#include "C_graph_algo.h"

double C_graph::PONF(int k, char *output_file) {
  int i, id, v_m, v_a;
  int v_m_opt;
  double v_a_lcc;
  int *temp_opt_list = new int[max_node + 1];
  int *v_m_opt_lit = new int[max_node + 1];
  FILE *fp;

  fp = fopen(output_file, "w");

  for (i = 0; i < k; i++) {
    returnMaxLCC();

#pragma omp parallel for
    for (id = 0; id <= max_node; id++) {
      if (nodes[id].is_existed && nodes[id].lcc == max_lcc) {
        nodes[id].optionality = returnOptionality(id);
      }
    }

    v_m_opt = max_node + 1;
    for (id = 0; id <= max_node; id++) {
      if (!nodes[id].is_existed || nodes[id].lcc < max_lcc) {
        continue;
      } else {
        if (nodes[id].optionality <= v_m_opt) {
          v_m = id;
          v_m_opt = nodes[id].optionality;
        }
      }
    }
    v_m_opt = returnOptionality(v_m, v_m_opt_lit);

    v_a_lcc = 0;
    for (int j = 0; j < v_m_opt; j++) {
      if (nodes[v_m_opt_lit[j]].lcc > v_a_lcc) {
        v_a_lcc = nodes[v_m_opt_lit[j]].lcc;
        v_a = v_m_opt_lit[j];
      }
    }

    // printf("\t %d %d \n", v_m, v_a);
    fprintf(fp, "%d %d\n", v_m, v_a);
    updateForAnNewEdge(v_m, v_a);
    addEdge(v_m, v_a);
    qsort(nodes[v_m].neighbors, nodes[v_m].degree, sizeof(int), cmp);
    qsort(nodes[v_a].neighbors, nodes[v_a].degree, sizeof(int), cmp);

    if (i % 100 == 0) {
      calculateAllNodeLCC();
    } else if (i % 1500 == 0) {
      calculateAllNodeLCC();
    }

    // test();
  }

  delete[] temp_opt_list;
  delete[] v_m_opt_lit;
  fclose(fp);
  returnMaxLCC();
  return max_lcc;
}
