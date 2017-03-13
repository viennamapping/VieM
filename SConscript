#/******************************************************************************
# * SConscript
# *
# * Source of KaHIP -- Karlsruhe High Quality Partitioning.
# *
# ******************************************************************************
# * Copyright (C) 2015 Christian Schulz <christian.schulz@kit.edu>
# *
# * This program is free software: you can redistribute it and/or modify it
# * under the terms of the GNU General Public License as published by the Free
# * Software Foundation, either version 2 of the License, or (at your option)
# * any later version.
# *
# * This program is distributed in the hope that it will be useful, but WITHOUT
# * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# * more details.
# *
# * You should have received a copy of the GNU General Public License along with
# * this program.  If not, see <http://www.gnu.org/licenses/>.
# *****************************************************************************/


# The main SConscript file for the code.
#
# We simply import the main environment and then define the targets.  This
# submodule contains a sequential matching and contraction code and tests for
# the code.
import platform
import sys

# Get the current platform.
SYSTEM = platform.uname()[0]

Import('env')

# Build a library from the code in lib/.
libkaffpa_files = [   'extern/KaHIP/lib/data_structure/graph_hierarchy.cpp',
                      'extern/KaHIP/lib/algorithms/strongly_connected_components.cpp',
                      'extern/KaHIP/lib/algorithms/topological_sort.cpp',
                      'extern/KaHIP/lib/algorithms/push_relabel.cpp',
                      'extern/KaHIP/lib/tools/quality_metrics.cpp',
                      'extern/KaHIP/lib/tools/random_functions.cpp',
                      'extern/KaHIP/lib/tools/graph_extractor.cpp',
                      'extern/KaHIP/lib/tools/misc.cpp',
                      'extern/KaHIP/lib/tools/partition_snapshooter.cpp',
                      'extern/KaHIP/lib/partition/graph_partitioner.cpp',
                      'extern/KaHIP/lib/partition/w_cycles/wcycle_partitioner.cpp',
                      'extern/KaHIP/lib/partition/coarsening/coarsening.cpp',
                      'extern/KaHIP/lib/partition/coarsening/contraction.cpp',
                      'extern/KaHIP/lib/partition/coarsening/edge_rating/edge_ratings.cpp',
                      'extern/KaHIP/lib/partition/coarsening/matching/matching.cpp',
                      'extern/KaHIP/lib/partition/coarsening/matching/random_matching.cpp',
                      'extern/KaHIP/lib/partition/coarsening/matching/gpa/path.cpp',
                      'extern/KaHIP/lib/partition/coarsening/matching/gpa/gpa_matching.cpp',
                      'extern/KaHIP/lib/partition/coarsening/matching/gpa/path_set.cpp',
                      'extern/KaHIP/lib/partition/coarsening/clustering/node_ordering.cpp',
                      'extern/KaHIP/lib/partition/coarsening/clustering/size_constraint_label_propagation.cpp',
                      'extern/KaHIP/lib/partition/initial_partitioning/initial_partitioning.cpp',
                      'extern/KaHIP/lib/partition/initial_partitioning/initial_partitioner.cpp',
                      'extern/KaHIP/lib/partition/initial_partitioning/initial_partition_bipartition.cpp',
                      'extern/KaHIP/lib/partition/initial_partitioning/initial_refinement/initial_refinement.cpp',
                      'extern/KaHIP/lib/partition/initial_partitioning/bipartition.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/uncoarsening.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/separator/vertex_separator_algorithm.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/separator/vertex_separator_flow_solver.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/greedy_neg_cycle.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/problem_factory.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/mixed_refinement.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/label_propagation_refinement/label_propagation_refinement.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/refinement.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/2way_fm_refinement/two_way_fm.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/two_way_flow_refinement.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/boundary_bfs.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/flow_solving_kernel/cut_flow_problem_solver.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/most_balanced_minimum_cuts/most_balanced_minimum_cuts.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_refinement.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/complete_boundary.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/partial_boundary.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/quotient_graph_scheduling.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/simple_quotient_graph_scheduler.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/active_block_quotient_graph_scheduler.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_core.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_commons.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph_fabric.cpp', 
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/advanced_models.cpp', 
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/multitry_kway_fm.cpp', 
                      'extern/KaHIP/lib/algorithms/cycle_search.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/cycle_refinement.cpp',
                      'extern/KaHIP/lib/parallel_mh/galinier_combine/gal_combine.cpp',
                      'extern/KaHIP/lib/parallel_mh/galinier_combine/construct_partition.cpp',
                      'extern/KaHIP/lib/partition/uncoarsening/refinement/tabu_search/tabu_search.cpp'
                      ]
                       
libmapping                = ['lib/mapping/local_search_mapping.cpp',
                             'lib/mapping/full_search_space.cpp',
                             'lib/mapping/full_search_space_pruned.cpp',
                             'lib/mapping/communication_graph_search_space.cpp',
                             'lib/io/graph_io.cpp',
                             'lib/mapping/fast_construct_mapping.cpp',
                             'lib/mapping/construct_distance_matrix.cpp',
                             'lib/mapping/mapping_algorithms.cpp',
                             'lib/mapping/construct_mapping.cpp' ]


if env['program'] == 'viem':
        env.Append(CXXFLAGS = '-DMODE_VIEM')
        env.Append(CCFLAGS  = '-DMODE_VIEM')
        env.Program('viem', ['app/viem.cpp']+libkaffpa_files+libmapping, LIBS=['libargtable2','gomp'])

if env['program'] == 'generate_model':
        env.Append(CXXFLAGS = '-DMODE_GENERATE_MODEL')
        env.Append(CCFLAGS  = '-DMODE_GENERATE_MODEL')
        env.Program('generate_model', ['app/generate_model.cpp']+libkaffpa_files+libmapping, LIBS=['libargtable2','gomp'])

if env['program'] == 'graphchecker':
        env.Append(CXXFLAGS = '-DMODE_GRAPHCHECKER')
        env.Append(CCFLAGS  = '-DMODE_GRAPHCHECKER')
        env.Program('graphchecker', ['app/graphchecker.cpp'], LIBS=['libargtable2','gomp'])

if env['program'] == 'evaluator':
        env.Append(CXXFLAGS = '-DMODE_EVALUATOR')
        env.Append(CCFLAGS  = '-DMODE_EVALUATOR')
        env.Program('evaluator', ['app/evaluator.cpp']+libkaffpa_files+libmapping, LIBS=['libargtable2','gomp'])

