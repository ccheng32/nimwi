#include "C_node.h"
#include <cstring>

C_node::C_node() {
  degree = 0;
  is_existed = 0;
  is_updated = 0;
  lcc = -1;
  length_neighbor = 0;
  upper_bound = NULL;
  neighbors = NULL;
  optionality = 0;
}

C_node::~C_node() {
  if (is_existed) {
    if (upper_bound != NULL) delete[] upper_bound;
    if (neighbors != NULL) delete[] neighbors;
    // printf("%p\n",neighbors);
  }
}

int C_node::hasNeighbor(int id) {
  int left = 0, right = degree;
  int mid;
  while (left < right - 1) {
    mid = (left + right) / 2;
    if (neighbors[mid] == id) {
      return 1;
    } else if (neighbors[mid] < id) {
      left = mid;
    } else {
      right = mid;
    }
  }

  if (neighbors[left] == id) return 1;

  return 0;
}
