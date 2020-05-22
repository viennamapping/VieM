/******************************************************************************
 * evaluator.cpp 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 *****************************************************************************/

#include <argtable3.h>
#include <regex.h>
#include <string.h> 

#include "data_structure/graph_access.h"
#include "data_structure/matrix/online_distance_matrix.h"
#include "graph_io.h"
#include "macros_assertions.h"
#include "parse_parameters.h"
#include "partition/partition_config.h"
#include "quality_metrics.h"

int main(int argn, char **argv) {

        PartitionConfig config;
        std::string graph_filename;

        bool is_graph_weighted = false;
        bool suppress_output   = false;
        bool recursive         = false;
       
        int ret_code = parse_parameters(argn, argv, 
                                        config, 
                                        graph_filename, 
                                        is_graph_weighted, 
                                        suppress_output, recursive); 

        if(ret_code) {
                return 0;
        }

        graph_access C;     
        graph_io::readGraphWeighted(C, graph_filename);

        std::cout <<  "model has " << C.number_of_nodes() << " nodes, " << C.number_of_edges() <<  " edges "  << std::endl;
        std::cout <<  "specified hierarchy has " <<  config.k << " entities."  << std::endl;

        if(config.k != C.number_of_nodes() ) {
                std::cout <<  "The number of nodes of the model does not match specified system entities. Please provide correct input parameters (using the --hierarchy_parameter_string option)."  << std::endl;
                exit(0);
        }

        C.set_partition_count(config.k); 
 
        std::vector<PartitionID> input_mapping;
        if(config.input_partition != "") {
                std::cout <<  "reading input mapping" << std::endl;
                graph_io::readMapping(input_mapping, config.input_partition);
                if(input_mapping.size() != C.number_of_nodes()) {
                        std::cout <<  "The number of nodes of the model does not match size of the mapping specified as input."  << std::endl;
                        exit(0);
                }
        } else {
                std::cout <<  "Please specify an input mapping using the --input_mapping flag."  << std::endl;
                exit(0);
        }

        online_distance_matrix D(config.k, config.k);
        D.setPartitionConfig(config);

        quality_metrics qm;
        NodeWeight qap = qm.total_qap(C, D, input_mapping);
        std::cout <<  "quadratic assignment objective J(C,D,Pi') = " << qap << std::endl;



}
