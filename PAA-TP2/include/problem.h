#ifndef PROBLEM_H
#define PROBLEM_H

typedef enum {CELL_EMPTY, CELL_BLOCK, CELL_ANCHOR, CELL_MONSTER} CellType;

typedef struct {
    CellType type;
    int val;
    char raw[4];
} Cell;

typedef struct {
    int h, w;
    int F_init, D, N;
    Cell **map[2];
    char *source_filename;
} Problem;

Problem *problem_read_from_file(const char *path);
void problem_free(Problem *P);
int cell_is_passable(const Cell *c);

#endif
