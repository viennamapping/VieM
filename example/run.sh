#!/bin/bash


# generate a model of communication and computation from an input graph, e.g. for sparse matrix vector multiplication
# creates a file calles model.graph
../deploy/generate_model 3elt.graph --k 128 

# OUTPUT 
#io time: 0.017342
#block weight upper bound 38
#graph has 4720 nodes and 27444 edges
#performing partitioning! 
#partitioning took 2.87221
#model has 128 nodes, 630 edges 
#writing model of computation and communication to disk

# map the model onto a specified architecture (16*4*2 = 128)
# creates a file called permutation (e.g. the mapping)
../deploy/viem model.graph --hierarchy_parameter_string=16:4:2 --distance_parameter_string=1:10:100

# OUTPUT
#io time: 0.000907065
#model has 128 nodes, 630 edges 
#specified hierarchy has 128 entities.
#now computing mapping
#total num cores 128
#running fast hierarchy top down
#construction took 0.0319651
#J(C,D,Pi) = 30316
#local search took 0.00581491
#time spent for mapping 0.038717
#solution is a permutation
#quadratic assignment objective J(C,D,Pi') = 30316
#writing to mapping to disk

# evaluate again (for completeness)
../deploy/evaluator model.graph --input_mapping=permutation --hierarchy_parameter_string=16:4:2 --distance_parameter_string=1:10:100

# OUTPUT
#model has 128 nodes, 630 edges 
#specified hierarchy has 128 entities.
#reading input mapping
#quadratic assignment objective J(C,D,Pi') = 30316
