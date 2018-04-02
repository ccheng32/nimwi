#ifndef C_NODE
#define C_NODE

#include <unordered_set>

class C_node {
 private:
  std::unordered_set<int> neighbors;

 public:
  int is_existed, is_updated;
  double lcc;
  int optionality;
  double *upper_bound;

  // ctr destr
  C_node();
  ~C_node();

  // neighbors
  int hasNeighbor(int id);
  void addNeighbor(int id);
  std::unordered_set<int>::const_iterator first_neighbor() const;
  std::unordered_set<int>::const_iterator last_neighbor() const;

  int degree();
};
#endif
