#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dp_solver.h"
#include "problem.h"

#define INF_NEG -1000000000

typedef struct {
    int force;
    int prev_row, prev_col, prev_map;
    int valid;
} State;

/* Solve with DP similar to previous single-file solution */
PathStep *solve_problem_dp(const Problem *P, int *out_path_len, SolverStats *stats, int verbose) {
    int h = P->h, w = P->w;
    int F0 = P->F_init, D = P->D, N = P->N;

    clock_t t0 = clock();

    /* allocate dp */
    State ***dp = malloc(2 * sizeof(State**));
    for (int m=0;m<2;++m) {
        dp[m] = malloc(h * sizeof(State*));
        for (int i=0;i<h;++i) {
            dp[m][i] = malloc(w * sizeof(State));
            for (int j=0;j<w;++j) {
                dp[m][i][j].force = INF_NEG;
                dp[m][i][j].prev_row = -1;
                dp[m][i][j].prev_col = -1;
                dp[m][i][j].prev_map = -1;
                dp[m][i][j].valid = 0;
            }
        }
    }

    long nodes_expanded = 0;
    long reachable_states = 0;
    long max_frontier = 0;

    /* initialize column 0 */
    for (int m=0;m<2;++m) {
        for (int r=0;r<h;++r) {
            const Cell *c = &P->map[m][r][0];
            if (!cell_is_passable(c)) continue;
            int f = F0;
            if (c->type == CELL_EMPTY) f += D;
            else if (c->type == CELL_MONSTER) f -= c->val;
            else if (c->type == CELL_ANCHOR) {
                int m2 = 1 - m;
                if (!cell_is_passable(&P->map[m2][r][0])) continue;
                if (f > dp[m2][r][0].force) {
                    dp[m2][r][0].force = f;
                    dp[m2][r][0].valid = 1;
                }
                continue;
            }
            if (f <= 0) continue;
            if (f > dp[m][r][0].force) {
                dp[m][r][0].force = f;
                dp[m][r][0].valid = 1;
            }
        }
    }

    /* DP iterate */
    for (int col=0; col<w-1; ++col) {
        long frontier = 0;
        for (int m=0;m<2;++m) {
            for (int r=0;r<h;++r) {
                if (!dp[m][r][col].valid) continue;
                frontier++;
                nodes_expanded++;
                int cur_force = dp[m][r][col].force;
                for (int dr=-1; dr<=1; ++dr) {
                    int nr = r+dr, nc = col+1;
                    if (nr < 0 || nr >= h) continue;
                    const Cell *t = &P->map[m][nr][nc];
                    if (!cell_is_passable(t)) continue;
                    int nf = cur_force;
                    if (t->type == CELL_EMPTY) nf += D;
                    else if (t->type == CELL_MONSTER) nf -= t->val;
                    else if (t->type == CELL_ANCHOR) {
                        if (nf <= 0) continue;
                        int m2 = 1 - m;
                        if (!cell_is_passable(&P->map[m2][nr][nc])) continue;
                        if (nf > dp[m2][nr][nc].force) {
                            dp[m2][nr][nc].force = nf;
                            dp[m2][nr][nc].valid = 1;
                            dp[m2][nr][nc].prev_row = r;
                            dp[m2][nr][nc].prev_col = col;
                            dp[m2][nr][nc].prev_map = m;
                        }
                        continue;
                    }
                    if (nf <= 0) continue;
                    if (nf > dp[m][nr][nc].force) {
                        dp[m][nr][nc].force = nf;
                        dp[m][nr][nc].valid = 1;
                        dp[m][nr][nc].prev_row = r;
                        dp[m][nr][nc].prev_col = col;
                        dp[m][nr][nc].prev_map = m;
                    }
                }
            }
        }
        if (frontier > max_frontier) max_frontier = frontier;
    }

    /* find best at last column */
    int best_force = INF_NEG;
    int best_r=-1, best_m=-1;
    for (int m=0;m<2;++m) for (int r=0;r<h;++r) {
        if (dp[m][r][w-1].valid) {
            int f = dp[m][r][w-1].force;
            if (f > best_force) { best_force = f; best_r = r; best_m = m; }
        }
    }

    reachable_states = 0;
    for (int m=0;m<2;++m) for (int i=0;i<h;++i) for (int j=0;j<w;++j)
        if (dp[m][i][j].valid) reachable_states++;

    clock_t t1 = clock();
    double ms = 0.0;
    if (t1 > t0) ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;

    if (stats) {
        stats->nodes_expanded = nodes_expanded;
        stats->reachable_states = reachable_states;
        stats->max_frontier = max_frontier;
        stats->approx_ms = ms;
    }

    if (best_force == INF_NEG) {
        /* cleanup */
        for (int m=0;m<2;++m) for (int i=0;i<h;++i) free(dp[m][i]);
        for (int m=0;m<2;++m) free(dp[m]);
        free(dp);
        *out_path_len = 0;
        return NULL;
    }

    /* reconstruct path into array of PathStep (reverse then flip) */
    int maxlen = w * 2 + 10;
    PathStep *rev = malloc(maxlen * sizeof(PathStep));
    int plen = 0;
    int cm = best_m, cr = best_r, cc = w-1;
    while (cc >= 0) {
        rev[plen].row = cr;
        rev[plen].col = cc;
        rev[plen].map = cm;
        rev[plen].force = dp[cm][cr][cc].force;
        plen++;
        State *s = &dp[cm][cr][cc];
        if (s->prev_col == -1) break;
        int pr = s->prev_row, pc = s->prev_col, pm = s->prev_map;
        cr = pr; cc = pc; cm = pm;
    }

    /* flip to forward order */
    PathStep *path = malloc(plen * sizeof(PathStep));
    for (int i=0;i<plen;++i) path[i] = rev[plen-1-i];
    free(rev);

    /* free dp */
    for (int m=0;m<2;++m) for (int i=0;i<h;++i) free(dp[m][i]);
    for (int m=0;m<2;++m) free(dp[m]);
    free(dp);

    *out_path_len = plen;
    return path;
}

void free_pathsteps(PathStep *p) {
    if (p) free(p);
}
