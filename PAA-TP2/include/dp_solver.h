#ifndef DP_SOLVER_H
#define DP_SOLVER_H

#include "problem.h"

typedef struct {
    long nodes_expanded;
    long reachable_states;
    long max_frontier;
    double approx_ms;
} SolverStats;


typedef struct {
    int row;
    int col;
    int map; 
    int force;
} PathStep;

PathStep *solve_problem_dp(const Problem *P, int *out_path_len, SolverStats *stats, int verbose);
void free_pathsteps(PathStep *p);

#endif 
