#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_ID 80000
#define MAX_DEG 10000
#define TAU 0.6

/*Assume node exists and their id are positive integers*/

int cmp(const void *a, const void *b) { return (*(int *)a) - (*(int *)b); }

class C_graph {
 public:
  int max_node;
  double max_lcc;

  class C_node {
   public:
    int is_existed, is_updated;
    int degree;
    int *neighbors, length_neighbor;
    int optionality;
    double lcc;
    double *upper_bound;

    C_node() {
      degree = 0;
      is_existed = 0;
      is_updated = 0;
      lcc = -1;
      length_neighbor = 0;
      upper_bound = NULL;
      neighbors = NULL;
      optionality = 0;
    }

    ~C_node() {
      if (is_existed) {
        if (upper_bound != NULL) delete[] upper_bound;
        if (neighbors != NULL) delete[] neighbors;
        // printf("%p\n",neighbors);
      }
    }

    int hasNeighbor(int id) {
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
  };

  C_node *nodes;

  C_graph() {
    max_node = 0;
    max_lcc = 0;
    nodes = new C_node[MAX_ID]();
  }

  ~C_graph() {
    max_node = 0;
    delete[] nodes;
  }

  void addEdge(int id_1, int id_2) {
    if (!nodes[id_1].is_existed) {
      nodes[id_1].is_existed = 1;
      nodes[id_1].neighbors = new int[MAX_DEG];
      if (id_1 > max_node) max_node = id_1;

    } else if (nodes[id_1].length_neighbor == nodes[id_1].degree) {
      int *new_neighbors = new int[nodes[id_1].degree + 10000];
      memset(new_neighbors, 0x00, sizeof(int) * (nodes[id_1].degree + 10000));
      memcpy(new_neighbors, nodes[id_1].neighbors,
             sizeof(int) * (nodes[id_1].degree));
      nodes[id_1].length_neighbor = nodes[id_1].degree + 10000;

      delete[] nodes[id_1].neighbors;
      nodes[id_1].neighbors = new_neighbors;
    }

    if (!nodes[id_2].is_existed) {
      nodes[id_2].is_existed = 1;
      nodes[id_2].neighbors = new int[MAX_DEG];
      if (id_2 > max_node) max_node = id_2;
    } else if (nodes[id_2].length_neighbor == nodes[id_2].degree) {
      int *new_neighbors = new int[nodes[id_2].degree + 10000];
      memset(new_neighbors, 0x00, sizeof(int) * (nodes[id_2].degree + 10000));
      memcpy(new_neighbors, nodes[id_2].neighbors,
             sizeof(int) * (nodes[id_2].degree));
      nodes[id_2].length_neighbor = nodes[id_2].degree + 10000;

      delete[] nodes[id_1].neighbors;
      nodes[id_1].neighbors = new_neighbors;
    }

    nodes[id_2].neighbors[nodes[id_2].degree++] = id_1;
    nodes[id_1].neighbors[nodes[id_1].degree++] = id_2;

    // printf("%d %d %d %d\n", id_1,nodes[id_1].degree, id_2,
    // nodes[id_2].degree);
    return;
  }

  void inputGraph(char file_name[]) {
    int id_1, id_2;
    FILE *fp;
    fp = fopen(file_name, "r");

    while (fscanf(fp, "%d%d", &id_1, &id_2) == 2) {
      // printf("%d %d\n", id_1, id_2);
      addEdge(id_1, id_2);
    }

    fclose(fp);

    for (int i = 0; i <= max_node; i++) {
      if (nodes[i].is_existed) {
        qsort(nodes[i].neighbors, nodes[i].degree, sizeof(int), cmp);
      }
    }
    return;
  }

  void inputLCC(char file_name[]) {
    int id;
    double lcc;

    FILE *fp;
    fp = fopen(file_name, "r");

    while (fscanf(fp, "%d%lf", &id, &lcc) == 2) {
      nodes[id].lcc = lcc;
      // printf("%ld %.2lf\n",id, lcc);
    }

    fclose(fp);

    return;
  }

  // return value is the number of common neighbors, neighbors are stored in
  // common_neighbors
  int returnCommonNeighbors(int u, int v, int *common_neighbors) {
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

  double calculateOneNodeLCC(int id) {
    int pi, pn;
    int count = 0;

    if (nodes[id].degree == 1) {
      nodes[id].lcc = 0;
      // return nodes[id].lcc;
    } else {
      for (int i = 0; i < nodes[id].degree; i++) {
        int neighbor = nodes[id].neighbors[i];
        pi = 0;  // id
        pn = 0;  // neighbor[i]
        // check the overlap of neighbor lists of neighbor[i] and id

        while (pi < nodes[id].degree && pn < nodes[neighbor].degree) {
          // printf("\t%d %d\n", pi, pn);
          if (nodes[id].neighbors[pi] == nodes[neighbor].neighbors[pn]) {
            pi++;
            pn++;
            count++;
          } else if (nodes[id].neighbors[pi] < nodes[neighbor].neighbors[pn]) {
            pi++;
          } else {
            pn++;
          }
        }
        // printf("i %d  neighbor %d count %d\n", id, neighbor, count);
      }

      nodes[id].lcc = double(count) / nodes[id].degree / (nodes[id].degree - 1);
      // printf("id %d, lcc %lf %d\n", id, nodes[id].lcc, count);
    }
    return nodes[id].lcc;
  }

  void calculateAllNodeLCC() {
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

  double returnMaxLCC() {
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

  // call after the lcc has been calculated
  void calculateUpperBound(int k) {
    int id, i, k1;
    double current;

    for (id = 0; id <= max_node; id++) {
      if (nodes[id].is_existed) {
        nodes[id].upper_bound = new double[k + 1];

        int count =
            nodes[id].lcc * nodes[id].degree * (nodes[id].degree - 1) / 2;

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

  // obtain update after add edge (u, v)
  // call before the edge is added into the graph
  void updateForAnNewEdge(int u, int v) {
    int *common_neighbors = new int[nodes[u].degree];
    int n_common = returnCommonNeighbors(u, v, common_neighbors);
    int i;

// common neighbors;
#pragma omp parallel for
    for (i = 0; i < n_common; i++) {
      // printf("\t%d %.2lf\n",
      // common_neighbors[i],nodes[common_neighbors[i]].lcc);
      nodes[common_neighbors[i]].lcc += 2.0 /
                                        nodes[common_neighbors[i]].degree /
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

  // with acceleration
  void updateForAnNewEdgeA(int u, int v, int k) {
    int *common_neighbors = new int[nodes[u].degree];
    int n_common = returnCommonNeighbors(u, v, common_neighbors);
    int i;

// common neighbors;
#pragma omp parallel for
    for (i = 0; i < n_common; i++) {
      // printf("\t%d %.2lf\n",
      // common_neighbors[i],nodes[common_neighbors[i]].lcc);
      if (nodes[common_neighbors[i]].upper_bound[k] > max_lcc) {
        nodes[common_neighbors[i]].lcc +=
            2.0 / nodes[common_neighbors[i]].degree /
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

  int returnFarthestNodeFrom(int id) {
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

  int returnOptionality(int id, int *optional) {
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

  int returnOptionality(int id) {
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

  double BUM(int k, char *output_file) {
    int i, j;
    int v_m, v_a;
    int current_max;
    FILE *fp;

    clock_t t3, t2;

    fp = fopen(output_file, "w");

    t2 = clock();

    for (i = 1; i <= k; i++) {
      current_max = 0;
      for (j = 0; j < max_node; j++) {
        if (nodes[j].is_existed && nodes[j].lcc > current_max) {
          current_max = nodes[j].lcc;
          v_m = j;
        }
      }

      current_max = 0;
      for (j = 0; j < max_node; j++) {
        if (nodes[j].is_existed && j != v_m && !nodes[v_m].hasNeighbor(j) &&
            nodes[j].lcc > current_max) {
          // printf("* %d %d %d\n", v_m, j, nodes[v_m].hasNeighbor(j));
          current_max = nodes[j].lcc;
          v_a = j;
        }
      }

      // printf("\t %d %d \n", v_m, v_a);
      fprintf(fp, "%d %d\n", v_m, v_a);
      updateForAnNewEdge(v_m, v_a);
      addEdge(v_m, v_a);
      // test();
      qsort(nodes[v_m].neighbors, nodes[v_m].degree, sizeof(int), cmp);
      qsort(nodes[v_a].neighbors, nodes[v_a].degree, sizeof(int), cmp);

      if (i % 100 == 0) {
        calculateAllNodeLCC();
      } else if (i % 1500 == 0) {
        t3 = clock();
        calculateAllNodeLCC();
        printf("BUM\t%d\t%.2lf\t%.2lf\n", i, returnMaxLCC(),
               double(t3 - t2) / CLOCKS_PER_SEC);
      }
    }

    fclose(fp);
    returnMaxLCC();
    return max_lcc;
  }

  double SIM(int k, char *output_file) {
    int i, j;
    int v_m, v_a;
    int current_max;
    FILE *fp;

    clock_t t3, t2;

    fp = fopen(output_file, "w");

    t2 = clock();
    for (i = 1; i <= k; i++) {
      current_max = 0;
      for (j = 0; j < max_node; j++) {
        if (nodes[j].is_existed && nodes[j].lcc > current_max) {
          current_max = nodes[j].lcc;
          v_m = j;
        }
      }

      // printf("v_m %d\n", v_m);

      // find the farthest node
      v_a = returnFarthestNodeFrom(v_m);

      // printf("v_a %d\n", v_a);

      // printf("\t %d %d \n", v_m, v_a);
      fprintf(fp, "%d %d\n", v_m, v_a);

      updateForAnNewEdge(v_m, v_a);
      addEdge(v_m, v_a);
      qsort(nodes[v_m].neighbors, nodes[v_m].degree, sizeof(int), cmp);
      qsort(nodes[v_a].neighbors, nodes[v_a].degree, sizeof(int), cmp);

      if (i % 100 == 0) {
        calculateAllNodeLCC();
      } else if (i % 1500 == 0) {
        t3 = clock();
        calculateAllNodeLCC();
        printf("SIM\t%d\t%.2lf\t%.2lf\n", i, returnMaxLCC(),
               double(t3 - t2) / CLOCKS_PER_SEC);
      }
    }

    fclose(fp);
    // calculateAllNodeLCC();
    returnMaxLCC();
    return max_lcc;
  }

  double OISA(int k, char *output_file) {
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

  double PONF(int k, char *output_file) {
    int i, id, v_m, v_a;
    int v_m_opt;
    double v_a_lcc;
    int *temp_opt_list = new int[max_node + 1];
    int *v_m_opt_lit = new int[max_node + 1];
    FILE *fp;

    clock_t t3, t2;

    fp = fopen(output_file, "w");

    t2 = clock();
    for (i = 0; i < k; i++) {
      returnMaxLCC();

#pragma omp parallel for
      for (id = 0; id <= max_node; id++) {
        if (nodes[id].is_existed && nodes[id].lcc == max_lcc) {
          nodes[id].optionality = returnOptionality(id);
        }
      }

      v_m_opt = max_node + 1;
      for (id = 0; id <= max_node; id++) {
        if (!nodes[id].is_existed || nodes[id].lcc < max_lcc) {
          continue;
        } else {
          if (nodes[id].optionality <= v_m_opt) {
            v_m = id;
            v_m_opt = nodes[id].optionality;
          }
        }
      }
      v_m_opt = returnOptionality(v_m, v_m_opt_lit);

      v_a_lcc = 0;
      for (int j = 0; j < v_m_opt; j++) {
        if (nodes[v_m_opt_lit[j]].lcc > v_a_lcc) {
          v_a_lcc = nodes[v_m_opt_lit[j]].lcc;
          v_a = v_m_opt_lit[j];
        }
      }

      // printf("\t %d %d \n", v_m, v_a);
      fprintf(fp, "%d %d\n", v_m, v_a);
      updateForAnNewEdge(v_m, v_a);
      addEdge(v_m, v_a);
      qsort(nodes[v_m].neighbors, nodes[v_m].degree, sizeof(int), cmp);
      qsort(nodes[v_a].neighbors, nodes[v_a].degree, sizeof(int), cmp);

      if (i % 100 == 0) {
        calculateAllNodeLCC();
      } else if (i % 1500 == 0) {
        t3 = clock();
        calculateAllNodeLCC();
        printf("PONF\t%d\t%.2lf\t%.2lf\n", i, returnMaxLCC(),
               double(t3 - t2) / CLOCKS_PER_SEC);
      }

      // test();
    }

    delete[] temp_opt_list;
    delete[] v_m_opt_lit;
    fclose(fp);
    returnMaxLCC();
    return max_lcc;
  }

  void test() {
    int i, j;

    for (i = 0; i < 15; i++) {
      if (nodes[i].is_existed) {
        printf("id %d %.2lf:", i, nodes[i].lcc);
        for (j = 0; j < nodes[i].degree; j++)
          printf(" %d", nodes[i].neighbors[j]);
        printf("\n");
      }
    }

    /*
    for(i = 0; i < 15; i++){

        if (nodes[i].is_existed){
            calculateOneNodeLCC(i);

        }

    }
    */
  }

} graph;

// use updateForAnNewEdgeA
int main(int argc, char **argv) {
  int k;
  double obj;
  clock_t t2, t3;
  char output_file_name[64];

  if (argc != 5) {
    printf(
        "USAGE: <executable-file> <algorithm> <input-network> <lcc-file> "
        "<k>\n");
    return 0;
  }

  graph.inputGraph(argv[2]);
  sscanf(argv[4], "%d", &k);
  // graph.calculateAllNodeLCC();
  graph.inputLCC(argv[3]);
  sprintf(output_file_name, "%s_%s_%d.txt", argv[1], argv[2], k);
  // graph.calculateUpperBound(k);
  printf("initialization done %s\n", output_file_name);

  t2 = clock();
  if (strcmp(argv[1], "PONF") == 0) {
    // printf("***method ponf\n");
    obj = graph.PONF(k, output_file_name);
  } else if (strcmp(argv[1], "OISA") == 0) {
    // printf("***method ponf\n");
    obj = graph.OISA(k, output_file_name);
  } else if (strcmp(argv[1], "SIM") == 0) {
    // printf("***method sim\n");
    obj = graph.SIM(k, output_file_name);
  } else if (strcmp(argv[1], "BUM") == 0) {
    // printf("***method bum\n");
    obj = graph.BUM(k, output_file_name);
  } else {
    // printf("method wrong; re enter\n");
    return 0;
  }
  t3 = clock();

  // graph.test();
  printf("%s\t%d\t%.2lf\t%.2lf\n", argv[1], k, obj,
         double(t3 - t2) / CLOCKS_PER_SEC);
  return 0;
}
