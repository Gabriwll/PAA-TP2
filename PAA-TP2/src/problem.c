#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../include/problem.h"

Problem* initializeProblem(char* filePath){
    FILE* file;

    Problem* problem = (Problem*)malloc(sizeof(Problem));
    if(!problem) return NULL;

    printf("Tentando abrir arquivo: %s\n", filePath);
    if(!openFile(&file, filePath)){
        free(problem);
        return NULL;
    }

    if(!readHeader(&file, problem)){
        closeFile(file);
        free(problem);

        return NULL;
    }

    problem->map[0] = initializeGrid(problem->height, problem->width);
    if(!readMap(&file, problem->map[0], problem->height, problem->width)){
        closeFile(file);
        free(problem);

        return NULL;
    }

    if(!skipMapSeparator(&file)){
        closeFile(file);
        free(problem);

        return NULL;
    }
    
    problem->map[1] = initializeGrid(problem->height, problem->width);
    if(!readMap(&file, problem->map[1], problem->height, problem->width)){
        closeFile(file);
        free(problem);

        return NULL;
    }
    
    if (!problem->map[0] || !problem->map[1]){
        closeFile(file);
        freeProblem(problem);
        return NULL;
    }
    
    closeFile(file);
    return problem;
}

static Cell** initializeGrid(int height, int width){
    Cell** grid = (Cell**)malloc(height * sizeof(Cell*)); //FIXME: possível erro de sintaxe
    if(grid == NULL){
        printf("Erro ao alocar a estrutura inicial do grid.\n");
        return NULL;
    }

    for (int i = 0; i < height; ++i){
        grid[i] = (Cell*)malloc(width * sizeof(Cell));

        if(grid[i] == NULL){
            printf("Erro ao alocar a coluna %d no grid.\n", i);

            for(int j = 0; j < i; j++){
                free(grid[j]);
            }
            free(grid);
            
            return NULL;
        }
    }

    return grid;
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


static int openFile(FILE** file, const char* path){ //FIXME: Possível erro ao abrir o arquivo
    *file = fopen(path, "r");
    if(*file == NULL){
        printf("Erro na abertura de arquivo %s.\n", path);
        return 0;
    }
    
    return 1;
}

static void closeFile(FILE* file){
    fclose(file);
}

static int readHeader(FILE** file, Problem* problem){ //FIXME: não sei se tem necessidade desse ponteiro duplo no arquivo
    if (fscanf(*file, "%d %d %d %d %d",
               &problem->height,
               &problem->width,
               &problem->initialForce,
               &problem->restRecoverValue,
               &problem->nikadorForce) != 5){
        
        printf("Erro: cabeçalho inválido.\n");
        
        return 0;
    }

    return 1;
}

static int readMap(FILE** file, Cell** grid, int height, int width){ //FIXME: não sei se tem necessidade desse ponteiro duplo no arquivo
    char token[16];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (fscanf(*file, "%15s", token) != 1) {
                fprintf(stderr, "Erro: token faltando na posição [%d][%d]\n", i, j);
                return 0;
            }
            parse_token(&grid[i][j], token);
        }
    }

    return 1;
}

static int skipMapSeparator(FILE** file){ //FIXME: não sei se tem necessidade desse ponteiro duplo
    char separator[8];

    if (fscanf(*file, "%7s", separator) != 1 || strcmp(separator, "///") != 0) {
        fprintf(stderr, "Erro: separador '///' não encontrado.\n");

        return 0;
    }

    return 1;
}

void printMap(Problem* problem){
    printf("Caminho do presente:\n\n");
    printGrid(problem->map[0], problem->height, problem->width);
    
    printf("Caminho do futuro:\n\n");
    printGrid(problem->map[1], problem->height, problem->width);
}

static void printGrid(Cell** grid, int height, int width){
    printf("+");
    for(int i = 0; i < (width * 4) + 1; i++){
        printf("-");
    }
    printf("+\n");


    for(int i = 0; i < height; i++){
        printf("| ");
        for(int j = 0; j < width; j++){
            if(grid[i][j].type == CELL_EMPTY){
                printf("000 ");
            }
            
            if(grid[i][j].type == CELL_BLOCK){
                printf("*** ");
            }
            
            if(grid[i][j].type == CELL_ANCHOR){
                printf("AAA ");
            }
            
            if(grid[i][j].type == CELL_MONSTER){
                for(int k = 0; k < 3 - howMuchDigits(grid[i][j].val); k++) printf("0");

                printf("%d ", grid[i][j].val);
            }
        }
        printf("|\n");
    }

    printf("+");
    for(int i = 0; i < (width * 4) + 1; i++){
        printf("-");
    }
    printf("+\n");
}

static int howMuchDigits(int number){
    if(number == 0){
        return 1;
    }

    return (int)log10(abs(number)) + 1;
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
        printf("Grid nao existe.\n");
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
    Problem* problem = initializeProblem("./PAA-TP2/Files/In/exemplo.txt");
    if(problem == NULL){
        printf("Deu problema. Execucao terminada.\n");

        return 1;
    }

    printMap(problem);

    freeProblem(problem);

    return 0;
}