#include <cstdio>
#include <cstring>
#include <ctime>
#include "C_graph.h"

/*Assume node exists and their id are positive integers*/

// use updateForAnNewEdgeA
int main(int argc, char **argv) {
  int k;
  double obj;
  clock_t t2, t3;
  char output_file_name[64];
  C_graph graph;

  if (argc != 6) {
    printf(
        "USAGE: <executable-file> <algorithm> <input-network> <lcc-file> <k> "
        "<output-file>\n");
    return 0;
  }

  graph.inputGraph(argv[2]);
  sscanf(argv[4], "%d", &k);
  // graph.calculateAllNodeLCC();
  graph.inputLCC(argv[3]);
  sprintf(output_file_name, "%s", argv[5]);
  // graph.calculateUpperBound(k);
  printf("initialization done %s\n", output_file_name);

  t2 = clock();
  graph.nimwi(argv[1], k, output_file_name);
  t3 = clock();

  // graph.test();
  printf("%s\t%d\t%.2lf\t%.2lf\n", argv[1], k, obj,
         double(t3 - t2) / CLOCKS_PER_SEC);
  return 0;
}
