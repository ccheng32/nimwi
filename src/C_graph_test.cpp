#include "C_graph.h"
#include <cstdio>

void C_graph::test() {
  int i, j;

  for (i = 0; i < 15; i++) {
    if (nodes[i].is_existed) {
      printf("id %d %.2lf:", i, nodes[i].lcc);
      for (j = 0; j < nodes[i].degree; j++)
        printf(" %d", nodes[i].neighbors[j]);
      printf("\n");
    }
  }
}
