VieM v1.0
=====

VieM - Vienna Mapping and Sparse Quadratic Assignment -- is a family of mapping programs. It includes serveral heuristics to map a task graphs onto a specified processor graph. We provide them here as easy to use open source software. 

Main project site:
http://viem.taa.univie.ac.at

Installation Notes
=====


Just type ./compile_withcmake.sh. Once you did that you can try to run the following commands in the example folder:

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


Licence
=====

The program is licenced under MIT. 
If you publish results using our algorithms, please acknowledge our work by quoting the following paper:

```
@inproceedings{schulztraeff2017,
             AUTHOR = {Schulz, Christian and Träff, Jesper Larsson},
             TITLE = {{Better Process Mapping and Sparse Quadratic Assignment}},
             BOOKTITLE = {{Proceedings of the 16th International Symposium on Experimental Algorithms (SEA'17)}},
             SERIES = {{LIPIcs}},
             PUBLISHER = {Dagstuhl},
             VOLUME = {75},
             PAGES = {4:1 -- 4:15},
             NOTE = {Technical Report, arXiv:1702.04164},
             YEAR = {2017}
}
```
