#ifndef UTILS_H
#define UTILS_H

#include "dp_solver.h"
#include "problem.h"

typedef struct GraphInfo{
    char* graphTitle;
    
    char* xLabelName;
    char* yLabelName;
    
    int* xData;
    int* yData;
    int dataSize;
}GraphInfo;

typedef struct Data {
    int quantSolutionStored; // Armazena o total de vezes que um resultado foi salvo na memória
    int quantSolutionUsed;   // Armazena a quantidade de vezes que o resultado salvo na memória foi usado
    /*Com essas duas variáveis podemos plotar gráficos que relacionam a quantidade de vezes que um dado
    * resultado foi utilizad, o que pode ser útil para avaliar se a programação dinâmica foi a melhor
    * forma de resolver o problema proposto.
    */
    int recursiveCalls; //Chamadas recursivas realizadas pelo algoritmo

    int problemTimeElapsed; //Tempo de execução total durante a execução do problema geral
    int avgSubproblemTimeElapsed; //Tempo médio gasto pelas execuções dos subproblemas
} Data;

GraphInfo* initializeGraphInfo(char* graphTitle, int dataSize, char* xLabelName, int* xData, char* yLabelName, int* yData);
void print_stats(const SolverStats *s);
static char* buildPythonCommand(char* graphTitle, char* xLabelName, char* yLabelName);
void plotGraph(GraphInfo* graph);
void plotSolverStats(const SolverStats *stats, char* title);
void freeGraphInfo(GraphInfo* graph);

#endif  //UTILS_H
