#include "C_graph.h"
#include <cstdio>

void C_graph::test() {
  int i;

  for (i = 0; i < 15; i++) {
    if (nodes[i].is_existed) {
      printf("id %d %.2lf:", i, nodes[i].lcc);
      for (auto j = nodes[i].first_neighbor(); j != nodes[i].last_neighbor();
           j++)
        printf(" %d", *j);
      printf("\n");
    }
  }
}
