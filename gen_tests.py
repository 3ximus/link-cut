#!/usr/bin/python2
import sys
import random
from time import time

args = sys.argv

usage = args[0] + " <N nodes> <N operations>"
if len(args) != 3:
    print "Usage: " + usage
    exit(-1)

nodes = int(args[1])
ops   = int(args[2])

print "Generating file with %d nodes and %d operations." % (nodes, ops)
if nodes > ops:
    print "There have to be more ops than nodes"
    exit(-2)

possible_ops = ['C', 'Q', 'Q', 'L']
filename = "tests/custom/custom_%d_nodes_%d_ops" % (nodes, ops)

with open(filename, "w") as f:
    f.write("%d\n" % nodes)

    # start by linking all
    for i in xrange(1, nodes):
        f.write("L " + str(i) + " " + str(i+1) + "\n")

    # Cut, question, question, link
    random.seed(time())
    seq_len = len(possible_ops)
    for i in xrange(ops-nodes):
        r1 = random.randint(1, nodes)
        r2 = random.randint(1, nodes)
        while r2 == r1:
            r2 = random.randint(1, nodes)

        f.write("%s %d %d\n" % (possible_ops[i % seq_len], r1, r2))


print "Output written to file %s." % filename