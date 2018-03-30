#include <cstring>
#include <unordered_set>

#include "C_graph.h"
#include "C_graph_algo.h"

#define MAX_DEG 16
#define TAU 0.60

int cmp(const void *a, const void *b) { return (*(int *)a) - (*(int *)b); }

static void createNode(int id, int &max_node, int &num_nodes, C_node *nodes) {
  if (!nodes[id].is_existed) {
    num_nodes++;
    nodes[id].is_existed = 1;
    nodes[id].neighbors = new int[MAX_DEG];
    if (id > max_node) {
      max_node = id;
    }
  } else if (nodes[id].length_neighbor == nodes[id].degree) {
    int new_neighbor_length = nodes[id].degree << 1;
    int *new_neighbors = new int[new_neighbor_length];
    memset(new_neighbors, 0x00, sizeof(int) * (new_neighbor_length));
    memcpy(new_neighbors, nodes[id].neighbors,
           sizeof(int) * (nodes[id].degree));
    nodes[id].length_neighbor = new_neighbor_length;

    delete[] nodes[id].neighbors;
    nodes[id].neighbors = new_neighbors;
  }
}

void C_graph::addEdge(int id_1, int id_2) {
  createNode(id_1, max_node, num_nodes, nodes);
  createNode(id_2, max_node, num_nodes, nodes);

  nodes[id_2].neighbors[nodes[id_2].degree++] = id_1;
  nodes[id_1].neighbors[nodes[id_1].degree++] = id_2;

  // printf("%d %d %d %d\n", id_1,nodes[id_1].degree, id_2,
  // nodes[id_2].degree);
  return;
}

int C_graph::returnCommonNeighbors(int u, int v, int *common_neighbors) {
  int pu, pv;
  int count = 0;
  pu = 0;  // id
  pv = 0;  // neighbor[i]
  // check the overlap of neighbor lists of u and v
  while (pu < nodes[u].degree && pv < nodes[v].degree) {
    if (nodes[u].neighbors[pu] == nodes[v].neighbors[pv]) {
      common_neighbors[count++] = nodes[u].neighbors[pu];
      pu++;
      pv++;
    } else if (nodes[u].neighbors[pu] < nodes[v].neighbors[pv]) {
      pu++;
    } else {
      pv++;
    }
  }

  return count;
}

double C_graph::calculateOneNodeLCC(int id) {
  // # of possible edges between neighbors
  double node_degree = (double)nodes[id].degree;
  double max_num_edges = node_degree * (node_degree - 1) / 2.0;
  double exist_num_edges = 0.0;

  // create set of neighbors
  std::unordered_set<int> neighbors;
  for (int j = 0; j < nodes[id].degree; j++) {
    neighbors.insert(nodes[id].neighbors[j]);
  }

  // traverse every neighbor's neighbors to see if the
  // edges match
  for (int neighbor_id : neighbors) {
    int *neighbors_neighbors = nodes[neighbor_id].neighbors;
    int num_neighbors_neighbors = nodes[neighbor_id].degree;
    for (int k = 0; k < num_neighbors_neighbors; k++) {
      if (neighbors_neighbors[k] < neighbor_id &&
          neighbors.find(neighbors_neighbors[k]) != neighbors.end()) {
        exist_num_edges += 1.0;
      }
    }
  }

  nodes[id].lcc = exist_num_edges / max_num_edges;
  return nodes[id].lcc;
}

void C_graph::calculateAllNodeLCC() {
  int id;
  for (id = 0; id <= max_node; id++) {
    if (nodes[id].is_existed) {
      calculateOneNodeLCC(id);
      // printf("id %d, lcc %.2lf %d\n", id, nodes[id].lcc, nodes[id].degree);
      if (nodes[id].lcc >= max_lcc) max_lcc = nodes[id].lcc;
    }
  }

  return;
}
double C_graph::returnMaxLCC() {
  int id;
  double temp;

  temp = 0;
#pragma omp parallel for
  for (id = 0; id <= max_node; id++) {
    if (nodes[id].lcc > 1) {
      calculateOneNodeLCC(id);
      // printf("id %d");
    }
  }

  for (id = 0; id <= max_node; id++) {
    if (nodes[id].is_existed && nodes[id].lcc >= temp) {
      temp = nodes[id].lcc;
    }
  }

  max_lcc = temp;
  return max_lcc;
}

void C_graph::calculateUpperBound(int k) {
  int id, i, k1;
  double current;

  for (id = 0; id <= max_node; id++) {
    if (nodes[id].is_existed) {
      nodes[id].upper_bound = new double[k + 1];

      int count = nodes[id].lcc * nodes[id].degree * (nodes[id].degree - 1) / 2;

      for (i = 0; i <= k; i++) nodes[id].upper_bound[i] = 0;

      for (k1 = 0; k1 <= k; k1++) {
        for (i = k1; i <= k; i++) {
          current = (count + (i - k1) + k1 * nodes[id].degree) /
                    (nodes[id].degree + k) / (nodes[id].degree + k - 1) * 2;
          if (current > nodes[id].upper_bound[i])
            nodes[id].upper_bound[i] = current;
        }
      }
    }
  }

  return;
}
void C_graph::updateForAnNewEdge(int u, int v) {
  int *common_neighbors = new int[nodes[u].degree];
  int n_common = returnCommonNeighbors(u, v, common_neighbors);
  int i;

// common neighbors;
#pragma omp parallel for
  for (i = 0; i < n_common; i++) {
    // printf("\t%d %.2lf\n",
    // common_neighbors[i],nodes[common_neighbors[i]].lcc);
    nodes[common_neighbors[i]].lcc += 2.0 / nodes[common_neighbors[i]].degree /
                                      (nodes[common_neighbors[i]].degree - 1);
  }

  // u
  if (nodes[u].degree > 1)
    nodes[u].lcc *= nodes[u].degree * (nodes[u].degree - 1) / 2;
  else
    nodes[u].lcc = 0;
  nodes[u].lcc += n_common;
  nodes[u].lcc /= nodes[u].degree * (nodes[u].degree + 1) / 2;

  // v
  if (nodes[v].degree > 1)
    nodes[v].lcc *= nodes[v].degree * (nodes[v].degree - 1) / 2;
  else
    nodes[v].lcc = 0;
  nodes[v].lcc += n_common;
  nodes[v].lcc /= nodes[v].degree * (nodes[v].degree + 1) / 2;

  return;
}
void C_graph::updateForAnNewEdgeA(int u, int v, int k) {
  int *common_neighbors = new int[nodes[u].degree];
  int n_common = returnCommonNeighbors(u, v, common_neighbors);
  int i;

// common neighbors;
#pragma omp parallel for
  for (i = 0; i < n_common; i++) {
    // printf("\t%d %.2lf\n",
    // common_neighbors[i],nodes[common_neighbors[i]].lcc);
    if (nodes[common_neighbors[i]].upper_bound[k] > max_lcc) {
      nodes[common_neighbors[i]].lcc += 2.0 /
                                        nodes[common_neighbors[i]].degree /
                                        (nodes[common_neighbors[i]].degree - 1);
      nodes[common_neighbors[i]].is_updated = 1;
      if (nodes[common_neighbors[i]].lcc > max_lcc)
        max_lcc = nodes[common_neighbors[i]].lcc;
    } else {
      nodes[common_neighbors[i]].is_updated = 0;
      nodes[common_neighbors[i]].lcc =
          nodes[common_neighbors[i]].upper_bound[k];
    }
  }

  // u
  if (nodes[u].upper_bound[k] > max_lcc) {
    if (nodes[u].degree > 1)
      nodes[u].lcc *= nodes[u].degree * (nodes[u].degree - 1) / 2;
    nodes[u].lcc += n_common;
    nodes[u].lcc /= nodes[u].degree * (nodes[u].degree + 1) / 2;
    if (nodes[u].lcc > max_lcc) max_lcc = nodes[u].lcc;
    nodes[v].is_updated = 0;

  } else {
    nodes[u].is_updated = 0;
    nodes[u].lcc = nodes[u].upper_bound[k];
  }

  // v
  if (nodes[v].upper_bound[k] > max_lcc) {
    if (nodes[v].degree > 1)
      nodes[v].lcc *= nodes[v].degree * (nodes[v].degree - 1) / 2;
    nodes[v].lcc += n_common;
    nodes[v].lcc /= nodes[v].degree * (nodes[v].degree + 1) / 2;
    if (nodes[v].lcc > max_lcc) max_lcc = nodes[v].lcc;
    nodes[v].is_updated = 1;
  } else {
    nodes[v].is_updated = 0;
    nodes[v].lcc = nodes[v].upper_bound[k];
  }

  return;
}
int C_graph::returnFarthestNodeFrom(int id) {
  int *visited = new int[max_node + 1];
  int *queue = new int[max_node + 1];
  int head, tail, current, i;

  // unvisited = -1!!!
  memset(visited, 0xFF, sizeof(int) * (max_node + 1));

  head = 0;
  tail = 0;
  queue[tail++] = id;
  visited[id] = 0;
  while (head < tail) {
    current = queue[head];
    // printf("current %d\n", current);
    for (i = 0; i < nodes[current].degree; i++) {
      if (visited[nodes[current].neighbors[i]] == -1) {
        visited[nodes[current].neighbors[i]] = visited[current] + 1;
        queue[tail++] = nodes[current].neighbors[i];
      }
    }
    head++;
  }

  current = queue[head - 1];

  delete[] visited;
  delete[] queue;

  return current;
}

int C_graph::returnOptionality(int id, int *optional) {
  int *visited = new int[max_node + 1];
  int *queue = new int[max_node + 1];
  int head, tail, current, i, j, n_optioanl;
  int n_common, c;

  // unvisited = -1!!!
  memset(visited, 0xFF, sizeof(int) * (max_node + 1));

  head = 0;
  tail = 0;
  queue[tail++] = id;
  visited[id] = 0;
  while (head < tail && visited[head] < 3) {
    current = queue[head];
    // printf("current %d\n", current);
    for (i = 0; i < nodes[current].degree; i++) {
      if (visited[nodes[current].neighbors[i]] == -1) {
        visited[nodes[current].neighbors[i]] = visited[current] + 1;
        queue[tail++] = nodes[current].neighbors[i];
      }
    }
    head++;
  }

  n_optioanl = 0;
  for (j = 0; j <= max_node; j++) {
    if (!nodes[j].is_existed)  // exclude non-nodes
      continue;

    if (visited[j] == -1)  // distance farer than 3
      optional[n_optioanl++] = j;
    else {
      n_common = returnCommonNeighbors(id, j, queue);
      for (c = 0; c < n_common; c++) {
        if (nodes[queue[c]].lcc >= TAU) break;
      }
      if (c == n_common) optional[n_optioanl++] = j;
    }
  }

  delete[] visited;
  delete[] queue;

  return n_optioanl;
}

int C_graph::returnOptionality(int id) {
  int *visited = new int[max_node + 1];
  int *queue = new int[max_node + 1];
  int head, tail, current, i, j, n_optioanl;
  int n_common, c;

  // unvisited = -1!!!
  memset(visited, 0xFF, sizeof(int) * (max_node + 1));

  head = 0;
  tail = 0;
  queue[tail++] = id;
  visited[id] = 0;
  while (head < tail && visited[head] < 3) {
    current = queue[head];
    // printf("current %d\n", current);
    for (i = 0; i < nodes[current].degree; i++) {
      if (visited[nodes[current].neighbors[i]] == -1) {
        visited[nodes[current].neighbors[i]] = visited[current] + 1;
        queue[tail++] = nodes[current].neighbors[i];
      }
    }
    head++;
  }

  n_optioanl = 0;
  for (j = 0; j <= max_node; j++) {
    if (!nodes[j].is_existed)  // exclude non-nodes
      continue;

    if (visited[j] == -1)  // distance farer than 3
      n_optioanl++;
    else {
      n_common = returnCommonNeighbors(id, j, queue);
      for (c = 0; c < n_common; c++) {
        if (nodes[queue[c]].lcc >= TAU) break;
      }
      if (c == n_common) n_optioanl++;
    }
  }

  delete[] visited;
  delete[] queue;

  return n_optioanl;
}
