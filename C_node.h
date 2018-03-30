#ifndef C_NODE
#define C_NODE

class C_node {
 public:
  int is_existed, is_updated;
  int degree;
  int *neighbors, length_neighbor;
  int optionality;
  double lcc;
  double *upper_bound;

  C_node();
  ~C_node();
  int hasNeighbor(int id);
};
#endif
