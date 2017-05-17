#!/bin/bash

python gen_tests.py $1 $2

if [ $? -eq 0 ]; then
    FILENAME="tests/custom/custom_$1_nodes_$2_ops"
    echo "Running with $1 nodes and $2 operations!"
    echo "Running with $1 nodes and $2 operations!"
    echo "Running with $1 nodes and $2 operations!"
    ./run_tests.sh $FILENAME
fi