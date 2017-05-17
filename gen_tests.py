#!/usr/bin/python2
import sys
import random
from time import time

args = sys.argv

usage = args[0] + " <N nodes> <N operations>"
if len(args) != 3:
    print "Usage: " + usage
    exit(1)

nodes = int(args[1])
ops   = int(args[2])

print "Generating file with %d nodes and %d operations." % (nodes, ops)

possible_ops = ['L', 'C', 'Q']
filename = "tests/custom/custom_%d_nodes_%d_ops" % (nodes, ops)

with open(filename, "w") as f:
    f.write("%d\n" % nodes)

    random.seed(time())
    for i in xrange(ops):
        f.write("%s %d %d\n" % (random.choice(possible_ops), random.randint(1, nodes), random.randint(1, nodes)))


print "Output written to file %s." % filename