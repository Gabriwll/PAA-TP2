#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "problem.h"

Cell** initializeGrid(int height, int width){
    Cell** grid = (Cell**)malloc(height * sizeof(Cell*)); //FIXME: possível erro de sintaxe
    if(grid == NULL){
        printf("Erro ao alocar a estrutura inicial do grid.");
        return NULL;
    }

    for (int i = 0; i < height; ++i){
        grid[i] = (Cell*)malloc(width * sizeof(Cell));

        if(grid[i] == NULL){
            printf("Erro ao alocar a coluna %d no grid", i);

            return NULL;
        }
    }

    return grid;
}

Problem* allocateProblem(int height, int width){
    Problem *problem = malloc(sizeof(Problem));
    
    if(!problem) return NULL;

    problem->map[0] = initializeGrid(height, width);
    problem->map[1] = initializeGrid(height, width);

    if (!problem->map[0] || !problem->map[1]) {
        problem_free(problem);
        return NULL;
    }

    return problem;
}

int cell_is_passable(Cell *cell){
    if(cell->type != CELL_BLOCK){
        return 0;
    }
    
    return 1;
}

static void parse_token(Cell *cell, const char *tok){
    strncpy(cell->raw, tok, 3);
    cell->raw[3] = '\0';

    if (strcmp(tok, "000") == 0){
        cell->type = CELL_EMPTY;
        cell->val = 0;

    }else if (strcmp(tok, "***") == 0){
        cell->type = CELL_BLOCK;
        cell->val = 0;

    }else if (strcmp(tok, "AAA") == 0){
        cell->type = CELL_ANCHOR;
        cell->val = 0;

    }else{
        cell->type = CELL_MONSTER;
        cell->val = atoi(tok);
    }
}

int openFile(FILE** file, const char* path){ //FIXME: Possível erro ao abrir o arquivo
    file = fopen(path, "r");
    if(file == NULL){
        printf("Erro na abertura de arquivo %s.\n", path);
        return 0;
    }
    
    return 1;
}

void closeFile(FILE* file){
    fclose(file);
}

int readHeader(FILE* file, Problem* problem){
    if (fscanf(file, "%d %d %d %d %d",
               &problem->height,
               &problem->width,
               &problem->initialForce,
               &problem->restRecoverValue,
               &problem->nikadorForce) != 5){
        
        printf(stderr, "Erro: cabeçalho inválido.\n");
        
        return 0;
    }

    return 1;
}

static int readMap(FILE* file, Cell** grid, int height, int width){
    char token[16];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (fscanf(file, "%15s", token) != 1) {
                fprintf(stderr, "Erro: token faltando na posição [%d][%d]\n", i, j);
                return 0;
            }
            parse_token(&grid[i][j], token);
        }
    }

    return 1;
}

static int skipMapSeparator(FILE* file){
    char separator[8];

    if (fscanf(file, "%7s", separator) != 1 || strcmp(separator, "///") != 0) {
        fprintf(stderr, "Erro: separador '///' não encontrado.\n");

        return 0;
    }

    return 1;
}

/*O bloco seguinte contém 3 funções que resolvem um único problema.
* apenas a função kill problem precisa ser usada fora do escopo desse arquivo.
* As funções auxiliares possuem utilidade apeas de dessentralização do problema.    
*/
void killProblem(FILE* file, Problem* problem){
    if(file != NULL) fclose(file);
    if(problem != NULL) freeProblem(problem);
}

static void freeProblem(Problem *problem){
    if (!problem){
        return;
    }

    freeGrid(problem->map[0], problem->height);
    freeGrid(problem->map[1], problem->height);

    if (problem->source_filename){
        free(problem->source_filename);
    }

    free(problem);
}

static int freeGrid(Cell** grid, int height){
    if(!grid){
        printf("Grid nao existe.");
        return 0;
    }

    for(int i = 0; i < height; i++){
        free(grid[i]);
    }
    free(grid);

    return 1;
}

/*Testando o TAD:
* 
*/
int main(){
    int width = 10;
    int height = 10;

    Cell** grid = initializeGrid(height, width);
}

/*
Problem* readProblemFromFile(const char *path) {
    FILE *file;
    Problem *problem = malloc(sizeof(Problem));
    
    int height;
    int width;

    if(!openFile(file, path)){
        printf("Erro na abertura do arquivo.");

        return NULL;
    }

    if (!problem){
        printf("Erro na alocacao de memoria ()");
        fclose(file);
        return NULL;
    }
    
    if (fscanf(file, "%d %d %d %d %d", &height,
                                       &width,
                                       &problem->initialForce,
                                       &problem->restRecoverValue,
                                       &problem->nikadorForce) != 5) {

        fclose(file);
        free(problem);
        
        return NULL;
    }
    
    problem->height = height;
    problem->width = width;
    
    problem->map[0] = initializeGrid(height, width);
    problem->map[1] = initializeGrid(height, width);

    if (!problem->map[0] || !problem->map[1]){
        fclose(file);
        problem_free(problem);
        return NULL;
    }

    char tok[64];
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++) {
            if (fscanf(file, " %s", tok) != 1) {
                fclose(file);
                problem_free(problem);
                return NULL;
            }

            parse_token(&problem->map[0][i][j], tok);
        }
    }

    if(fscanf(file, "%s", tok) != 1){
        fclose(file);
        problem_free(problem);
        return NULL;
    }

    if(strcmp(tok, "///") != 0){
        fclose(file);
        problem_free(problem);
        return NULL;
    }

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (fscanf(file, "%s", tok) != 1){ 
            fclose(file);
            problem_free(problem);
            return NULL;
        }

        parse_token(&problem->map[1][i][j], tok);
        }
    }

    fclose(file);
    problem->source_filename = strdup(path);
    return problem;
}
*/