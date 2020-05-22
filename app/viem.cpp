/******************************************************************************
 * viem.cpp
 *
 * Source of VieM -- Vienna Mapping and Sparse Quadratic Assignment 
 *****************************************************************************/

#include <argtable2.h>
#include <iostream>
#include <math.h>
#include <regex.h>
#include <sstream>
#include <stdio.h>
#include <string.h> 

#include "balance_configuration.h"
#include "data_structure/graph_access.h"
#include "data_structure/matrix/normal_matrix.h"
#include "data_structure/matrix/online_distance_matrix.h"
#include "graph_io.h"
#include "macros_assertions.h"
#include "parse_parameters.h"
#include "partition/graph_partitioner.h"
#include "partition/partition_config.h"
#include "partition/uncoarsening/refinement/cycle_improvements/cycle_refinement.h"
#include "mapping/mapping_algorithms.h"
#include "quality_metrics.h"
#include "random_functions.h"
#include "timer.h"

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

        std::streambuf* backup = std::cout.rdbuf();
        std::ofstream ofs;
        ofs.open("/dev/null");
        if(suppress_output) {
                std::cout.rdbuf(ofs.rdbuf()); 
        }

        config.LogDump(stdout);

        timer_x t;
        graph_access C;
        graph_io::readGraphWeighted(C, graph_filename);
        std::cout << "io time: " << t.elapsed()  << std::endl;

        forall_nodes(C, node) {
                C.setNodeWeight(node, 1);
        } endfor

        t.restart();
        std::cout <<  "model has " << C.number_of_nodes() << " nodes, " << C.number_of_edges() <<  " edges "  << std::endl;
        std::cout <<  "specified hierarchy has " <<  config.k << " entities."  << std::endl;

        if(config.k != C.number_of_nodes() ) {
                std::cout <<  "The number of nodes of the model does not match specified system entities. Please provide correct input parameters (using the --hierarchy_parameter_string option)."  << std::endl;
                exit(0);
        }

        std::cout <<  "now computing mapping"  << std::endl;
        std::vector< NodeID > perm_rank(config.k);

        t.restart();
        quality_metrics qm;
        mapping_algorithms ma;

        EdgeWeight qap = 0;
        if( config.distance_construction_algorithm != DIST_CONST_HIERARCHY_ONLINE) {
                normal_matrix D(config.k, config.k);
                ma.construct_a_mapping(config, C, D, perm_rank);
                std::cout <<  "time spent for mapping " << t.elapsed()  << std::endl;
                qap = qm.total_qap(C, D, perm_rank );
        } else {
                online_distance_matrix D(config.k, config.k);
                D.setPartitionConfig(config);
                ma.construct_a_mapping(config, C, D, perm_rank);
                std::cout <<  "time spent for mapping " << t.elapsed()  << std::endl;
                qap = qm.total_qap(C, D, perm_rank );
        }
        // solution check 
        std::cout <<  "solution is ";
        std::vector< NodeID > tbsorted = perm_rank;
        std::sort( tbsorted.begin(), tbsorted.end() );
        for( unsigned int i = 0; i < tbsorted.size(); i++) {
                if( tbsorted[i] != i ) {
                        std::cout <<  "NOT a permutation"  << std::endl;
                        std::cout <<  tbsorted[i] <<  " " << i   << std::endl;
                        exit(0);
                }
        }
        std::cout <<  "a permutation"  << std::endl;
        std::cout <<  "quadratic assignment objective J(C,D,Pi') = " << qap << std::endl;
        std::cout <<  "writing to mapping to disk"  << std::endl;

        std::stringstream filename;
        if(!config.filename_output.compare("")) {
                filename << "permutation";
        } else {
                filename << config.filename_output;
        }

        graph_io::writeVector( perm_rank, filename.str());
        // ******************************* done mapping *****************************************       

        ofs.close();
        std::cout.rdbuf(backup);
}
