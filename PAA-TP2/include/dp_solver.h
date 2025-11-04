#ifndef DP_SOLVER_H
#define DP_SOLVER_H

#include "problem.h"

/* stats produced by solver */
typedef struct {
    long nodes_expanded;
    long reachable_states;
    long max_frontier;
    double approx_ms;
} SolverStats;

/* represent one step of path (row, col, map, force after entering) */
typedef struct {
    int row;
    int col;
    int map; /* 0 present, 1 past */
    int force;
} PathStep;

/* Solve problem; returns path (allocated array), path_len via out param.
   Caller must free(*path) (allocated with malloc).
   If no valid path found, returns NULL and path_len=0.
   Verbose controls internal logging for stats collection. */
PathStep *solve_problem_dp(const Problem *P, int *out_path_len, SolverStats *stats, int verbose);

/* utility to free PathStep* if needed */
void free_pathsteps(PathStep *p);

#endif /* DP_SOLVER_H */
