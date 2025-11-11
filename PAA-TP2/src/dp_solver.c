#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "dp_solver.h"
#include "problem.h"

#define INF_NEG -1000000000

typedef struct {
    int force;
    int prev_row, prev_col, prev_map;
    int valid;
} State;

static int anchors_aligned(const Problem *P) {
    for (int i = 0; i < P->h; ++i)
        for (int j = 0; j < P->w; ++j)
            if ((strncmp(P->map[0][i][j].raw, "AAA", 3) == 0) != (strncmp(P->map[1][i][j].raw, "AAA", 3) == 0))
                return 0;
    return 1;
}

static void find_nearest_anchor(const Problem *P, int other_map, int r, int c, int *out_r, int *out_c) {
    int best_r = -1, best_c = -1, best_d = 1000000000;
    for (int i = 0; i < P->h; ++i)
        for (int j = 0; j < P->w; ++j)
            if (strncmp(P->map[other_map][i][j].raw, "AAA", 3) == 0) {
                int d = abs(i - r) + abs(j - c);
                if (d < best_d) { best_d = d; best_r = i; best_c = j; }
            }
    *out_r = best_r; *out_c = best_c;
}

static void map_anchor_target(const Problem *P, int from_map, int r, int c, int *out_map, int *out_r, int *out_c, int aligned_mode) {
    int other = 1 - from_map;
    if (aligned_mode && strncmp(P->map[other][r][c].raw, "AAA", 3) == 0) {
        *out_map = other; *out_r = r; *out_c = c; return;
    }
    find_nearest_anchor(P, other, r, c, out_r, out_c);
    *out_map = other;
}

PathStep *solve_problem_dp(const Problem *P, int *out_path_len, SolverStats *stats, int verbose) {
    int h = P->h, w = P->w, F0 = P->F_init, D = P->D;
#ifdef _WIN32
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
#else
    clock_t t0 = clock();
#endif
    int aligned_mode = anchors_aligned(P);

    State ***dp = malloc(2 * sizeof(State**));
    for (int m = 0; m < 2; ++m) {
        dp[m] = malloc(h * sizeof(State*));
        for (int i = 0; i < h; ++i) {
            dp[m][i] = malloc(w * sizeof(State));
            for (int j = 0; j < w; ++j) {
                dp[m][i][j].force = INF_NEG;
                dp[m][i][j].prev_row = -1;
                dp[m][i][j].prev_col = -1;
                dp[m][i][j].prev_map = -1;
                dp[m][i][j].valid = 0;
            }
        }
    }

    long nodes_expanded = 0, max_frontier = 0;

    for (int m = 0; m < 2; ++m)
        for (int r = 0; r < h; ++r) {
            const Cell *c = &P->map[m][r][0];
            if (!cell_is_passable(c)) continue;
            int f = F0;
            if (c->type == CELL_EMPTY) f += D;
            else if (c->type == CELL_MONSTER) f -= c->val;
            else if (c->type == CELL_ANCHOR) {
                if (f <= 0) continue;
                int target_map = -1, tr = -1, tc = -1;
                map_anchor_target(P, m, r, 0, &target_map, &tr, &tc, aligned_mode);
                if (tr < 0 || tc < 0) continue;
                if (!cell_is_passable(&P->map[target_map][tr][tc])) continue;
                if (f > dp[target_map][tr][tc].force) {
                    dp[target_map][tr][tc].force = f;
                    dp[target_map][tr][tc].valid = 1;
                }
                continue;
            }
            if (f <= 0) continue;
            if (f > dp[m][r][0].force) {
                dp[m][r][0].force = f;
                dp[m][r][0].valid = 1;
            }
        }

    for (int col = 0; col < w - 1; ++col) {
        long frontier = 0;
        for (int m = 0; m < 2; ++m)
            for (int r = 0; r < h; ++r) {
                if (!dp[m][r][col].valid) continue;
                frontier++; nodes_expanded++;
                int cur_force = dp[m][r][col].force;
                for (int dr = -1; dr <= 1; ++dr) {
                    int nr = r + dr, nc = col + 1;
                    if (nr < 0 || nr >= h) continue;
                    const Cell *t = &P->map[m][nr][nc];
                    if (!cell_is_passable(t)) continue;
                    int nf = cur_force;
                    if (t->type == CELL_EMPTY) nf += D;
                    else if (t->type == CELL_MONSTER) nf -= t->val;
                    else if (t->type == CELL_ANCHOR) {
                        if (nf <= 0) continue;
                        int target_map = -1, tr = -1, tc = -1;
                        map_anchor_target(P, m, nr, nc, &target_map, &tr, &tc, aligned_mode);
                        if (tr < 0 || tc < 0) continue;
                        if (!cell_is_passable(&P->map[target_map][tr][tc])) continue;
                        if (nf > dp[target_map][tr][tc].force) {
                            dp[target_map][tr][tc].force = nf;
                            dp[target_map][tr][tc].valid = 1;
                            dp[target_map][tr][tc].prev_row = r;
                            dp[target_map][tr][tc].prev_col = col;
                            dp[target_map][tr][tc].prev_map = m;
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
        if (frontier > max_frontier) max_frontier = frontier;
    }

    int best_force = INF_NEG, best_r = -1, best_m = -1;
    for (int m = 0; m < 2; ++m)
        for (int r = 0; r < h; ++r)
            if (dp[m][r][w - 1].valid) {
                int f = dp[m][r][w - 1].force;
                if (f > best_force) { best_force = f; best_r = r; best_m = m; }
            }

    long reachable_states = 0;
    for (int m = 0; m < 2; ++m)
        for (int i = 0; i < h; ++i)
            for (int j = 0; j < w; ++j)
                if (dp[m][i][j].valid) reachable_states++;

#ifdef _WIN32
    QueryPerformanceCounter(&end);
    double ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / (double)freq.QuadPart;
#else
    clock_t t1 = clock();
    double ms = (double)(t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
#endif

    if (stats) {
        stats->nodes_expanded = nodes_expanded;
        stats->reachable_states = reachable_states;
        stats->max_frontier = max_frontier;
        stats->approx_ms = ms;
    }

    if (best_force == INF_NEG) {
        for (int m = 0; m < 2; ++m) for (int i = 0; i < h; ++i) free(dp[m][i]);
        for (int m = 0; m < 2; ++m) free(dp[m]);
        free(dp);
        *out_path_len = 0;
        return NULL;
    }

    int maxlen = w * 2 + 10;
    PathStep *rev = malloc(maxlen * sizeof(PathStep));
    int plen = 0;
    int cm = best_m, cr = best_r, cc = w - 1;
    while (cc >= 0 && plen < maxlen) {
        rev[plen].row = cr;
        rev[plen].col = cc;
        rev[plen].map = cm;
        rev[plen].force = dp[cm][cr][cc].force;
        plen++;
        State *s = &dp[cm][cr][cc];
        if (s->prev_col == -1) break;
        cr = s->prev_row; cc = s->prev_col; cm = s->prev_map;
    }

    for (int m = 0; m < 2; ++m) for (int i = 0; i < h; ++i) free(dp[m][i]);
    for (int m = 0; m < 2; ++m) free(dp[m]);
    free(dp);

    PathStep *path = malloc(plen * sizeof(PathStep));
    for (int i = 0; i < plen; ++i) path[i] = rev[plen - 1 - i];
    free(rev);

    *out_path_len = plen;
    return path;
}

void free_pathsteps(PathStep *p) { if (p) free(p); }
