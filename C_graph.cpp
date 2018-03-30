#include "C_graph.h"
#define MAX_ID 80000

C_graph::C_graph() {
  max_node = 0;
  max_lcc = 0;
  nodes = new C_node[MAX_ID]();
}

C_graph::~C_graph() {
  max_node = 0;
  delete[] nodes;
}
