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

Problem* initializeProblem(char* filePath);
static Cell** initializeGrid(int height, int width);

int cell_is_passable(Cell *cell);
static void parse_token(Cell* cell, const char *tok);

static int openFile(FILE** file, const char* path);
static void closeFile(FILE* file);

static int readHeader(FILE** file, Problem* problem);
static int readMap(FILE** file, Cell** grid, int height, int width);
static int skipMapSeparator(FILE** file);

void printMap(Problem* problem); //Encapsula a função IprintGrid para melhorar a leitura do código
static void printGrid(Cell** grid, int height, int width);

static int howMuchDigits(int number); //Função auxiliar

void killProblem(FILE* file, Problem* problem); //Libera memória usada
void freeProblem(Problem* problem);
static int freeGrid(Cell** grid, int height);


#endif //PROBLEM_H
