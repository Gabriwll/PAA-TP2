#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/dp_solver.h"
#include "../include/problem.h"

#define INF_NEG -1000000000

typedef struct {
    int force;

    int prev_row;
    int prev_col;
    int prev_map;
    
    int valid;
} State;

PathStep *solve_problem_dp(const Problem *problem, int *out_path_len, SolverStats *stats, int verbose) {
    int height = problem->height;
    int width = problem->width;

    int F0 = problem->initialForce;
    int restRecoverValue = problem->restRecoverValue;
    int nikatorForce = problem->nikadorForce;

    clock_t t0 = clock();
    State*** dp = (State***)malloc(2 * sizeof(State**));
    
    for (int m = 0; m < 2; m++) {
        dp[m] = malloc(height * sizeof(State*));
    
        for (int i = 0; i < height; i++) {
            dp[m][i] = (State*)malloc(width * sizeof(State));
    
            for (int j = 0; j < width; j++) {
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
    
    for (int m = 0; m < 2; m++) {
        for (int r = 0; r < height; r++) {
            Cell* cell = &problem->map[m][r][0];
    
            if (!cell_is_passable(cell)){
                continue;
            }
    
            int f = F0;
    
            if (cell->type == CELL_EMPTY) f += restRecoverValue;
            else if (cell->type == CELL_MONSTER) f -= cell->val;
            else if (cell->type == CELL_ANCHOR) {
                int m2 = 1 - m;
    
                if (!cell_is_passable(&problem->map[m2][r][0])) continue;
    
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

    for (int col = 0; col < (width - 1); col++) {
        long frontier = 0;
    
        for (int m = 0; m < 2; m++) {
            for (int r = 0; r < height; r++) {
                if (!dp[m][r][col].valid) continue;
                frontier++;
                nodes_expanded++;
    
                int cur_force = dp[m][r][col].force;
    
                for (int dr = -1; dr <= 1; dr++) {
                    int nr = r+dr, nc = col+1;
    
                    if (nr < 0 || nr >= height) continue;
    
                    Cell *t = &problem->map[m][nr][nc];
    
                    if (!cell_is_passable(t)) continue;
    
                    int nf = cur_force;
    
                    if (t->type == CELL_EMPTY) nf += restRecoverValue;
                    else if (t->type == CELL_MONSTER) nf -= t->val;
                    else if (t->type == CELL_ANCHOR) {
                        if (nf <= 0) continue;
    
                        int m2 = 1 - m;
    
                        if (!cell_is_passable(&problem->map[m2][nr][nc])) continue;
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


    int best_force = INF_NEG;
    int best_r=-1, best_m=-1;

    for (int m=0;m<2;++m) for (int r=0;r<height;++r) {
        if (dp[m][r][width-1].valid) {
            int f = dp[m][r][width-1].force;
            if (f > best_force) {
                best_force = f;
                best_r = r;
                best_m = m;
            }
        }
    }

    reachable_states = 0;

    for (int m = 0; m < 2; m++){
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                if (dp[m][i][j].valid){
                    reachable_states++;
                }
            }
        }
    }

    clock_t t1 = clock();
    double ms = 0.0;

    if (t1 > t0){
        ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
    }

    if (stats) {
        stats->nodes_expanded = nodes_expanded;
        stats->reachable_states = reachable_states;
        stats->max_frontier = max_frontier;
        stats->approx_ms = ms;
    }

    if (best_force == INF_NEG) {
        /* cleanup */
        for (int m = 0; m < 2; m++){
            for (int i = 0; i < height; i++){
                free(dp[m][i]);
            }
        }

        for (int m = 0; m < 2; m++){
            free(dp[m]);
        }

        free(dp);
        *out_path_len = 0;
        
        return NULL;
    }

    int maxlen = width * 2 + 10;
    PathStep *rev = malloc(maxlen * sizeof(PathStep));
    int plen = 0;
    int cm = best_m, cr = best_r, cc = width - 1;
    
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
    
    PathStep *path = malloc(plen * sizeof(PathStep));

    for (int i = 0; i < plen; i++){
        path[i] = rev[plen - i - 1];
    }

    free(rev);

    for(int m = 0; m < 2; m++){
        for(int i = 0; i < height; i++){
            free(dp[m][i]);
        }
    }

    for(int m = 0; m < 2; m++){
        free(dp[m]);
    }

    free(dp);

    *out_path_len = plen;
    
           return path;
}

void free_pathsteps(PathStep *p) {
    if (p) free(p);
}
