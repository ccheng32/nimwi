#include "C_graph.h"
#include <cstring>

double C_graph::nimwi(char* algo, int k, char* output_file_name) {
  double obj = 0.0;

  if (strcmp(algo, "PONF") == 0) {
    // printf("***method ponf\n");
    obj = this->PONF(k, output_file_name);
  } else if (strcmp(algo, "OISA") == 0) {
    // printf("***method ponf\n");
    obj = this->OISA(k, output_file_name);
  } else if (strcmp(algo, "SIM") == 0) {
    // printf("***method sim\n");
    obj = this->SIM(k, output_file_name);
  } else if (strcmp(algo, "BUM") == 0) {
    // printf("***method bum\n");
    obj = this->BUM(k, output_file_name);
  }

  return obj;
}
