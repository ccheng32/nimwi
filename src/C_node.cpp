#include "C_node.h"
#include <cstring>

C_node::C_node() {
  is_existed = 0;
  is_updated = 0;
  lcc = 0.0;
  upper_bound = NULL;
  neighbors.clear();
  optionality = 0;
}

C_node::~C_node() {
  if (is_existed) {
    if (upper_bound != NULL) delete[] upper_bound;
    neighbors.clear();
    // printf("%p\n",neighbors);
  }
}

int C_node::hasNeighbor(int id) {
  return neighbors.find(id) == neighbors.end() ? 0 : 1;
}

void C_node::addNeighbor(int id) { neighbors.insert(id); }
std::unordered_set<int>::const_iterator C_node::first_neighbor() const {
  return neighbors.cbegin();
}
std::unordered_set<int>::const_iterator C_node::last_neighbor() const {
  return neighbors.cend();
}

int C_node::degree() { return neighbors.size(); }
