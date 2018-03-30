#include "C_graph.h"
#include <cmath>
#include "C_graph_algo.h"

double C_graph::OISA(int k, char *output_file) {
  int id, i, max_d, k_G;
  double l, a, b, c, temp;
  double min_max_lcc, return_;
  char file_name[128];

  max_d = 0;
  for (id = 0; id <= max_node; id++) {
    if (nodes[id].is_existed && nodes[id].degree > max_d) {
      max_d = nodes[id].degree;
    }
  }

  max_d = (max_d - 1) * max_d / 2;
  min_max_lcc = 1;
  for (i = 1; i <= max_d; i++) {
    k_G = 0;

    l = (i + 0.00000001) / max_d;
    for (id = 0; id <= max_node; id++) {
      if (k_G >= 2 * k) break;

      if (nodes[id].is_existed && nodes[id].lcc >= l) {
        a = l;
        b = 2 * l * nodes[id].degree - l;
        c = (nodes[id].degree - 1) * nodes[id].degree *
            (l * nodes[id].degree - nodes[id].lcc);

        temp = sqrt(b * b - 4 * a * c);

        if (temp > 0) {
          k_G += int((-b + temp) / 2 / a + 0.000001);
          // printf("%d: %lf %lf\n", id, b*b-4*a*c, temp);
        }
        // printf("%d %d\n", id, k_G);
      }
    }

    if (k_G < 2 * k && k_G > 0) {
      sprintf(file_name, "%.2lf_%s", l, output_file);
      return_ = PONF(k, file_name);

      if (return_ < min_max_lcc) {
        min_max_lcc = return_;
        // printf("*%lf %lf\n", l, return_);
      }
    }

    if (k_G == 0) break;
  }
  // printf("***%lf\n", min_max_lcc);
  return min_max_lcc;
}
