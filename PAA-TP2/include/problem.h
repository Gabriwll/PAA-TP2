#ifndef PROBLEM_H
#define PROBLEM_H

typedef enum CellType {
    CELL_EMPTY, 
    CELL_BLOCK, 
    CELL_ANCHOR, 
    CELL_MONSTER
}CellType;

typedef struct Cell {
    CellType type;
    int val;   
    char raw[4];
} Cell;

typedef struct Problem{
    Cell **map[2]; 
    char *source_filename;

    int height;
    int width;

    int initialForce;
    int restRecoverValue;
    int nikadorForce;
} Problem;

Cell** initializeGrid(int height, int width);
Problem* allocateProblem(int height, int width);

int cell_is_passable(Cell *cell);
static void parse_token(Cell* cell, const char *tok);

int openFile(FILE** file, const char* path);
void closeFile(FILE* file);

int readHeader(FILE* file, Problem* problem);
static int readMap(FILE* file, Cell** grid, int height, int width);
static int skipMapSeparator(FILE* file);


void killProblem(FILE* file, Problem* problem); //Libera memória usada
static void freeProblem(Problem* problem);
static int freeGrid(Cell** grid, int height);


#endif //PROBLEM_H
