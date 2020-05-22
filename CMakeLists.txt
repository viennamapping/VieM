cmake_minimum_required(VERSION 3.10)
include(CheckCXXCompilerFlag)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_program(CCACHE_PROGRAM ccache)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

if(CCACHE_PROGRAM)
  message(STATUS "Using compiler cache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CCACHE_PROGRAM}")
endif()
project(VieClus C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# if no build mode is specified build in release mode
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# tweak compiler flags
CHECK_CXX_COMPILER_FLAG(-funroll-loops COMPILER_SUPPORTS_FUNROLL_LOOPS)
if(COMPILER_SUPPORTS_FUNROLL_LOOPS)
  add_definitions(-funroll-loops)
endif()
CHECK_CXX_COMPILER_FLAG(-fno-stack-limit COMPILER_SUPPORTS_FNOSTACKLIMITS)
if(COMPILER_SUPPORTS_FNOSTACKLIMITS)
  add_definitions(-fno-stack-limit)
endif()
CHECK_CXX_COMPILER_FLAG(-Wall COMPILER_SUPPORTS_WALL)
if(COMPILER_SUPPORTS_WALL)
  add_definitions(-Wall)
endif()
CHECK_CXX_COMPILER_FLAG(-march=native COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
  add_definitions(-march=native)
endif()
CHECK_CXX_COMPILER_FLAG(-fpermissive COMPILER_SUPPORTS_FPERMISSIVE)
if(COMPILER_SUPPORTS_FPERMISSIVE)
  add_definitions(-fpermissive)
endif()

# check dependencies
#find_package(MPI REQUIRED)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  message(STATUS "OpenMP support detected")
  add_definitions(${OpenMP_CXX_FLAGS})
else()
  message(WARNING "OpenMP not available, activating workaround")
  add_library(OpenMP::OpenMP_CXX IMPORTED INTERFACE)
  set_property(TARGET OpenMP::OpenMP_CXX PROPERTY INTERFACE_COMPILE_OPTIONS "")
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/misc)
endif()

# 64 Bit option
option(64BITMODE "64 bit mode" OFF)
if(64BITMODE)
  add_definitions("-DMODE64BITEDGES")
  add_definitions("-DPOINTER64=1")
endif()

# optimized output
option(OPTIMIZED_OUTPUT "optimized output" OFF)
if(OPTIMIZED_OUTPUT)
  add_definitions("-DKAFFPAOUTPUT")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/app)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/argtable3-3.0.3)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/io)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/app)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/io)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extern/KaHIP/lib/tools)



set(LIBKAFFPA_SOURCE_FILES
extern/KaHIP/lib/data_structure/graph_hierarchy.cpp
extern/KaHIP/lib/algorithms/strongly_connected_components.cpp
extern/KaHIP/lib/algorithms/topological_sort.cpp
extern/KaHIP/lib/algorithms/push_relabel.cpp
extern/KaHIP/lib/tools/quality_metrics.cpp
extern/KaHIP/lib/tools/random_functions.cpp
extern/KaHIP/lib/tools/graph_extractor.cpp
extern/KaHIP/lib/tools/misc.cpp
extern/KaHIP/lib/tools/partition_snapshooter.cpp
extern/KaHIP/lib/partition/graph_partitioner.cpp
extern/KaHIP/lib/partition/w_cycles/wcycle_partitioner.cpp
extern/KaHIP/lib/partition/coarsening/coarsening.cpp
extern/KaHIP/lib/partition/coarsening/contraction.cpp
extern/KaHIP/lib/partition/coarsening/edge_rating/edge_ratings.cpp
extern/KaHIP/lib/partition/coarsening/matching/matching.cpp
extern/KaHIP/lib/partition/coarsening/matching/random_matching.cpp
extern/KaHIP/lib/partition/coarsening/matching/gpa/path.cpp
extern/KaHIP/lib/partition/coarsening/matching/gpa/gpa_matching.cpp
extern/KaHIP/lib/partition/coarsening/matching/gpa/path_set.cpp
extern/KaHIP/lib/partition/coarsening/clustering/node_ordering.cpp
extern/KaHIP/lib/partition/coarsening/clustering/size_constraint_label_propagation.cpp
extern/KaHIP/lib/partition/initial_partitioning/initial_partitioning.cpp
extern/KaHIP/lib/partition/initial_partitioning/initial_partitioner.cpp
extern/KaHIP/lib/partition/initial_partitioning/initial_partition_bipartition.cpp
extern/KaHIP/lib/partition/initial_partitioning/initial_refinement/initial_refinement.cpp
extern/KaHIP/lib/partition/initial_partitioning/bipartition.cpp
extern/KaHIP/lib/partition/uncoarsening/uncoarsening.cpp
extern/KaHIP/lib/partition/uncoarsening/separator/vertex_separator_algorithm.cpp
extern/KaHIP/lib/partition/uncoarsening/separator/vertex_separator_flow_solver.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/greedy_neg_cycle.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/problem_factory.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/mixed_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/label_propagation_refinement/label_propagation_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/2way_fm_refinement/two_way_fm.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/two_way_flow_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/boundary_bfs.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/flow_solving_kernel/cut_flow_problem_solver.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/flow_refinement/most_balanced_minimum_cuts/most_balanced_minimum_cuts.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/complete_boundary.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/partial_boundary.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/quotient_graph_scheduling.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/simple_quotient_graph_scheduler.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/quotient_graph_refinement/quotient_graph_scheduling/active_block_quotient_graph_scheduler.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_core.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/kway_graph_refinement_commons.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/augmented_Qgraph_fabric.cpp 
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/advanced_models.cpp 
extern/KaHIP/lib/partition/uncoarsening/refinement/kway_graph_refinement/multitry_kway_fm.cpp 
extern/KaHIP/lib/algorithms/cycle_search.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/cycle_improvements/cycle_refinement.cpp
extern/KaHIP/lib/parallel_mh/galinier_combine/gal_combine.cpp
extern/KaHIP/lib/parallel_mh/galinier_combine/construct_partition.cpp
extern/KaHIP/lib/partition/uncoarsening/refinement/tabu_search/tabu_search.cpp
extern/argtable3-3.0.3/argtable3.c)
add_library(libkaffpa OBJECT ${LIBKAFFPA_SOURCE_FILES})

set(LIBMAPPING_SOURCE_FILES
lib/mapping/local_search_mapping.cpp
lib/mapping/full_search_space.cpp
lib/mapping/full_search_space_pruned.cpp
lib/mapping/communication_graph_search_space.cpp
lib/io/graph_io.cpp
lib/mapping/fast_construct_mapping.cpp
lib/mapping/construct_distance_matrix.cpp
lib/mapping/mapping_algorithms.cpp
lib/mapping/construct_mapping.cpp)
add_library(libmapping OBJECT ${LIBMAPPING_SOURCE_FILES})

add_executable(generate_model app/generate_model.cpp $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libmapping>)
target_compile_definitions(generate_model PRIVATE "-DMODE_GENERATE_MODEL")
target_include_directories(generate_model PUBLIC )
target_link_libraries(generate_model ${OpenMP_CXX_LIBRARIES})
install(TARGETS generate_model DESTINATION bin)

add_executable(viem app/viem.cpp $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libmapping>)
target_compile_definitions(viem PRIVATE "-DMODE_VIEM")
target_include_directories(viem PUBLIC )
target_link_libraries(viem ${OpenMP_CXX_LIBRARIES})
install(TARGETS viem DESTINATION bin)

add_executable(graphchecker app/graphchecker.cpp $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libmapping>)
target_compile_definitions(graphchecker PRIVATE "-DMODE_GRAPHCHECKER")
target_include_directories(graphchecker PUBLIC )
target_link_libraries(graphchecker ${OpenMP_CXX_LIBRARIES})
install(TARGETS graphchecker DESTINATION bin)

add_executable(evaluator app/evaluator.cpp $<TARGET_OBJECTS:libkaffpa> $<TARGET_OBJECTS:libmapping>)
target_compile_definitions(evaluator PRIVATE "-DMODE_EVALUATOR")
target_include_directories(evaluator PUBLIC )
target_link_libraries(evaluator ${OpenMP_CXX_LIBRARIES})
install(TARGETS evaluator DESTINATION bin)


