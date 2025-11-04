#ifndef PROBLEM_H
#define PROBLEM_H

typedef enum {CELL_EMPTY, CELL_BLOCK, CELL_ANCHOR, CELL_MONSTER} CellType;

typedef struct {
    CellType type;
    int val;    /* for monsters: damage; otherwise 0 */
    char raw[4];
} Cell;

typedef struct {
    int h, w;
    int F_init, D, N;
    Cell **map[2]; /* map[0]=present, map[1]=past */
    char *source_filename;
} Problem;

/* read and free */
Problem *problem_read_from_file(const char *path);
void problem_free(Problem *P);

/* helpers */
int cell_is_passable(const Cell *c);

#endif /* PROBLEM_H */
