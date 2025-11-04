#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "problem.h"

static Cell **alloc_grid(int h, int w) {
    Cell **g = malloc(h * sizeof(Cell*));
    if (!g) return NULL;
    for (int i = 0; i < h; ++i) g[i] = malloc(w * sizeof(Cell));
    return g;
}

static void free_grid(Cell **g, int h) {
    if (!g) return;
    for (int i = 0; i < h; ++i) free(g[i]);
    free(g);
}

int cell_is_passable(const Cell *c) {
    return c->type != CELL_BLOCK;
}

static void parse_token(Cell *c, const char *tok) {
    strncpy(c->raw, tok, 3);
    c->raw[3] = '\0';
    if (strcmp(tok, "000") == 0) {
        c->type = CELL_EMPTY; c->val = 0;
    } else if (strcmp(tok, "***") == 0) {
        c->type = CELL_BLOCK; c->val = 0;
    } else if (strcmp(tok, "AAA") == 0) {
        c->type = CELL_ANCHOR; c->val = 0;
    } else {
        c->type = CELL_MONSTER;
        c->val = atoi(tok);
    }
}

Problem *problem_read_from_file(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) return NULL;
    Problem *P = malloc(sizeof(Problem));
    if (!P) { fclose(fp); return NULL; }
    int h,w;
    if (fscanf(fp, "%d %d %d %d %d", &h, &w, &P->F_init, &P->D, &P->N) != 5) {
        fclose(fp); free(P); return NULL;
    }
    P->h = h; P->w = w;
    P->map[0] = alloc_grid(h,w);
    P->map[1] = alloc_grid(h,w);
    if (!P->map[0] || !P->map[1]) { fclose(fp); problem_free(P); return NULL; }

    char tok[64];
    for (int i=0;i<h;++i) for (int j=0;j<w;++j) {
        if (fscanf(fp, "%s", tok) != 1) { fclose(fp); problem_free(P); return NULL; }
        parse_token(&P->map[0][i][j], tok);
    }
    if (fscanf(fp, "%s", tok) != 1) { fclose(fp); problem_free(P); return NULL; }
    if (strcmp(tok, "///") != 0) { fclose(fp); problem_free(P); return NULL; }
    for (int i=0;i<h;++i) for (int j=0;j<w;++j) {
        if (fscanf(fp, "%s", tok) != 1) { fclose(fp); problem_free(P); return NULL; }
        parse_token(&P->map[1][i][j], tok);
    }
    fclose(fp);
    P->source_filename = strdup(path);
    return P;
}

void problem_free(Problem *P) {
    if (!P) return;
    free_grid(P->map[0], P->h);
    free_grid(P->map[1], P->h);
    if (P->source_filename) free(P->source_filename);
    free(P);
}
