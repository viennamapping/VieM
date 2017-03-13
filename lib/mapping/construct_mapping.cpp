/******************************************************************************
 * construct_mapping.cpp
 *
 * Source of VieM -- Vienna Mapping and Sparse Quadratic Assignment 
 ******************************************************************************
 * Copyright (C) 2017 Christian Schulz 
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <cstdio>

#include "construct_mapping.h"
#include "tools/random_functions.h"
#include "fast_construct_mapping.h"
#include "data_structure/priority_queues/maxNodeHeap.h"


construct_mapping::construct_mapping() {

}

construct_mapping::~construct_mapping() {

}

void construct_mapping::construct_initial_mapping( PartitionConfig & config, graph_access & C, matrix & D, std::vector< NodeID > & perm_rank) {
        switch( config.construction_algorithm ) {
                case MAP_CONST_IDENTITY:
                        std::cout <<  "running identity mapping"  << std::endl;
                        construct_identity( config, C, D, perm_rank);
                        break;
                case MAP_CONST_RANDOM:
                        std::cout <<  "running random initial mapping"  << std::endl;
                        construct_random( config, C, D, perm_rank);
                        break;
                case MAP_CONST_OLDGROWING_FASTER:
                        std::cout <<  "running faster growing"  << std::endl;
                        construct_old_growing_faster( config, C, D, perm_rank);
                        break;
                case MAP_CONST_FASTHIERARCHY_BOTTOMUP:
                        std::cout <<  "running fast hierarchy bottom up"  << std::endl;
                        construct_fast_hierarchy_bottomup( config, C, D, perm_rank);
                        break;
                case MAP_CONST_FASTHIERARCHY_TOPDOWN:
                        std::cout <<  "running fast hierarchy top down"  << std::endl;
                        construct_fast_hierarchy_topdown( config, C, D, perm_rank);
                        break;
                default: 
                        std::cout <<  "running identity mapping"  << std::endl;
                        construct_identity( config, C, D, perm_rank);
        }
}


void construct_mapping::construct_old_growing_faster( PartitionConfig & config, graph_access & C, matrix & D, std::vector< NodeID > & perm_rank) {
        std::cout <<  "constructing initial mapping with faster growing"  << std::endl;

        //initialze perm rank
        //interpretation task 'node' is assinged to perm_rank[node] 
        for( unsigned int i = 0; i < perm_rank.size(); i++) {
                perm_rank[i] = UNASSIGNED;
        }

        maxNodeHeap unassigned_tasks; // contains unassigned tasks and their priority
        NodeWeight max_vol      = 0;
        NodeWeight max_vol_elem = 0;
        forall_nodes(C, node) {
                NodeWeight cur_vol = 0;
                forall_out_edges(C, e, node) {
                        cur_vol += C.getEdgeWeight(e);
                } endfor

                unassigned_tasks.insert( node, 0);

                if( cur_vol > max_vol ) {
                        max_vol = cur_vol;
                        max_vol_elem = node;
                }
        } endfor

        NodeWeight min_dist      = std::numeric_limits< NodeWeight >::max();
        NodeWeight min_dist_elem = 0;
        for( unsigned int cpu = 0; cpu < C.number_of_nodes(); cpu++) {
                NodeWeight cur_dist = 0;
                for( unsigned int cpu_bar = 0; cpu_bar < C.number_of_nodes(); cpu_bar++) {
                        cur_dist += D.get_xy( cpu, cpu_bar );
                }

                if( cur_dist < min_dist ) {
                        min_dist = cur_dist;
                        min_dist_elem = cpu;
                }
        }

        std::vector< NodeWeight > core_assigned( C.number_of_nodes(), UNASSIGNED);
        std::vector< NodeWeight > total_vol( C.number_of_nodes(), 0); // store volume to assigned nodes
        std::vector< NodeWeight > total_dist( C.number_of_nodes(), 0);

        std::vector< NodeWeight > unassigned_PEs;
        for( unsigned int cpu = 0; cpu < C.number_of_nodes(); cpu++) {
                unassigned_PEs.push_back(cpu);
        }

        perm_rank[max_vol_elem]      = min_dist_elem;
        core_assigned[min_dist_elem] = ASSIGNED;

        std::swap(unassigned_PEs[min_dist_elem], unassigned_PEs[unassigned_PEs.size()-1 ]);
        unassigned_PEs.pop_back();
        unassigned_tasks.deleteNode(max_vol_elem);

        for( unsigned int i = 0; i < unassigned_PEs.size(); i++) {
                NodeID cpu      = unassigned_PEs[i];
                total_dist[cpu] += D.get_xy( min_dist_elem, cpu );
        }

        forall_out_edges(C, e, max_vol_elem) {
                NodeID target_task = C.getEdgeTarget(e);
                if( unassigned_tasks.contains(target_task) ) {
                        total_vol[target_task] += C.getEdgeWeight(e);
                        unassigned_tasks.changeKey(target_task, total_vol[target_task]);
                }
        } endfor

        while( unassigned_tasks.size() > 0 ) {
                NodeID cur_task = unassigned_tasks.deleteMax();

                min_dist      = std::numeric_limits< NodeWeight >::max();
                min_dist_elem = std::numeric_limits< NodeWeight >::max();
                unsigned int idx = 0;
                for( unsigned int i = 0; i < unassigned_PEs.size(); i++) {
                        NodeID cpu      = unassigned_PEs[i];
                        if( core_assigned[cpu] == ASSIGNED ) { 
                                continue;
                        }
                        if( total_dist[cpu] <= min_dist ) {
                                min_dist = total_dist[cpu];
                                min_dist_elem = cpu;
                                idx = i;
                        }
                }
                NodeID cur_PE = min_dist_elem;

                perm_rank[cur_task]   = cur_PE;
                core_assigned[cur_PE] = ASSIGNED;

                //update priorities
                forall_out_edges(C, e, cur_task) {
                        NodeID target_task = C.getEdgeTarget(e);
                        if( unassigned_tasks.contains(target_task) ) {
                                total_vol[target_task] += C.getEdgeWeight(e);
                                unassigned_tasks.changeKey(target_task, total_vol[target_task]);
                        }
                } endfor

                std::swap(unassigned_PEs[idx], unassigned_PEs[unassigned_PEs.size()-1 ]);
                unassigned_PEs.pop_back();

                // update the priority of the other PEs
                for( unsigned int i = 0; i < unassigned_PEs.size(); i++) {
                        NodeID cpu      = unassigned_PEs[i];
                        total_dist[cpu] += D.get_xy( cur_PE, cpu );
                }
        } 
}

void construct_mapping::construct_identity( PartitionConfig & config, graph_access & C, matrix & D, std::vector< NodeID > & perm_rank) {
        for( unsigned int i = 0; i < perm_rank.size(); i++) {
                perm_rank[i] = i;
        }
}

void construct_mapping::construct_random( PartitionConfig & config, graph_access & C, matrix & D, std::vector< NodeID > & perm_rank) {
        construct_identity( config, C, D, perm_rank);
        random_functions::permutate_vector_good( perm_rank, false);
}

void construct_mapping::construct_fast_hierarchy_bottomup( PartitionConfig & config, graph_access & C, matrix & D, std::vector< NodeID > & perm_rank) {
        fast_construct_mapping fcm;
        fcm.construct_initial_mapping_bottomup( config, C, D, perm_rank);
}

void construct_mapping::construct_fast_hierarchy_topdown( PartitionConfig & config, graph_access & C, matrix & D, std::vector< NodeID > & perm_rank) {
        fast_construct_mapping fcm;
        fcm.construct_initial_mapping_topdown( config, C, D, perm_rank);
}
