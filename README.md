VieM v1.0
=====

VieM - Vienna Mapping and Sparse Quadratic Assignment -- is a family of mapping programs. It includes serveral heuristics to map a task graphs onto a specified processor graph. We provide them here as easy to use open source software. 

Main project site:
http://viem.taa.univie.ac.at

Installation Notes
=====

Before you can start you need to install the following software packages:

- Scons (http://www.scons.org/)
- Argtable (http://argtable.sourceforge.net/)

Once you installed the packages, just type ./compile.sh. Once you did that you can try to run the following commands in the example folder:

*//generate a model of communication and computation from an input graph, e.g. for sparse matrix vector multiplication*

*//creates a file calles model.graph*

../deploy/generate_model 3elt.graph --k 128 

*//map the model onto a specified architecture*

*//creates a file called permutation (e.g. the mapping)*

../deploy/viem model.graph --hierarchy_parameter_string=16:4:2 --distance_parameter_string=1:10:100

*//evaluate again (for completeness)*

../deploy/evaluator model.graph --input_mapping=permutation --hierarchy_parameter_string=16:4:2 --distance_parameter_string=1:10:100

For a description of the graph format please have look into the manual.

Travis-CI Status [![Build Status](https://travis-ci.org/schulzchristian/VieM.svg?branch=master)](https://travis-ci.org/schulzchristian/viem)
