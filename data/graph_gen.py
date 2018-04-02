#!/usr/bin/python
import sys
import os
import networkx as nx
import random

NUM = 100000000
if len(sys.argv) != 3 + 1:
  print 'usage: ./py [number of nodes] [edge probability] [output file name]'
  sys.exit(1)

nv = int(sys.argv[1])
ra = float(sys.argv[2])

# output file
f = open(sys.argv[3], 'w')

print 'generating graph g'
G = nx.fast_gnp_random_graph(nv, ra)
edgelist = nx.generate_edgelist(G, data=False)

for line in edgelist:
  a = int(line.split()[0])
  b = int(line.split()[1])
  f.write(str(a + 1) + ' ' + str(b + 1) + '\n')
f.close()

