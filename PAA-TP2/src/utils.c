#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utils.h"

GraphInfo* initializeGraphInfo(char* graphTitle, int dataSize, 
                               char* xLabelName, int* xData,
                               char* yLabelName, int* yData) {

    GraphInfo* graph = (GraphInfo*)malloc(sizeof(GraphInfo));
    if(graph == NULL){
        printf("Erro ao alocar memoria para graph.\n");

        return NULL;
    }

    graph->graphTitle = strdup(graphTitle);
    graph->dataSize = dataSize;

    graph->xLabelName = strdup(xLabelName);
    graph->xData = (int*)malloc(sizeof(int) * dataSize);
    graph->xData = xData;
    
    graph->yLabelName = strdup(yLabelName);
    graph->yData = (int*)malloc(sizeof(int) * dataSize);
    graph->yData = yData;

    return graph;
}

void print_stats(const SolverStats *s) {
    if (!s) return;
    printf("=== Estatísticas do solver ===\n");
    printf("Nós expandidos: %ld\n", s->nodes_expanded);
    printf("Estados alcançáveis: %ld\n", s->reachable_states);
    printf("Maior fronteira observada (col por col): %ld\n", s->max_frontier);
    printf("Tempo aproximado: %.3f ms\n", s->approx_ms);
    printf("=============================\n");
}

static char* buildPythonCommand(char* graphTitle, char* xLabelName, char* yLabelName) {
    // Calcula o tamanho necessário
    size_t len = snprintf(NULL, 0, "python ./PAA-TP2/src/plot.py --title \"%s\" --xlabel \"%s\" --ylabel \"%s\"", 
                         graphTitle, xLabelName, yLabelName);
    
    // Aloca e constrói o comando
    char* command = (char*)malloc(len + 1);
    if (!command) return NULL;
    
    snprintf(command, len + 1, "python ./PAA-TP2/src/plot.py --title \"%s\" --xlabel \"%s\" --ylabel \"%s\"", 
             graphTitle, xLabelName, yLabelName);
    
    return command;
}

void plotGraph(GraphInfo* graph) {
    if (!graph || !graph->xData || !graph->yData || graph->dataSize <= 0) {
        printf("Erro: Dados do gráfico inválidos.\n");
        return;
    }
    
    // Constrói o comando Python
    char* command = buildPythonCommand(
        graph->graphTitle ? graph->graphTitle : "Gráfico",
        graph->xLabelName ? graph->xLabelName : "Eixo X",
        graph->yLabelName ? graph->yLabelName : "Eixo Y"
    );
    
    if (!command) {
        printf("Erro ao construir comando.\n");
        return;
    }
    
    printf("Executando: %s\n", command);
    
    // Executa o Python
    FILE* pythonPlot = popen(command, "w");
    if (pythonPlot == NULL) {
        printf("Erro ao executar Python.\n");
        free(command);
        return;
    }
    
    // Envia dados para o Python
    for (int i = 0; i < graph->dataSize; i++) {
        fprintf(pythonPlot, "%d %d\n", graph->xData[i], graph->yData[i]);
    }

    fprintf(pythonPlot, "e\n");
    fflush(pythonPlot);
    
    // Limpeza
    int status = pclose(pythonPlot);
    free(command);
    
    printf("Plot finalizado com status: %d\n", status);
}

void plotSolverStats(const SolverStats *stats, char* title) {
    if (!stats) {
        printf("Erro: Estatísticas inválidas.\n");
        return;
    }

    // Dados para o gráfico (x = índice, y = valores)
    int dataSize = 4;
    int xData[] = {1, 2, 3, 4};
    int yData[] = {
        stats->nodes_expanded,
        stats->reachable_states, 
        stats->max_frontier,
        stats->approx_ms
    };

    // Criar estrutura do gráfico
    GraphInfo* graph = initializeGraphInfo(title ? title : "Estatísticas do Solver",
                                           dataSize,
                                           "Métricas",
                                           xData,
                                           "Valores",
                                           yData
    );

    if (!graph) {
        printf("Erro ao criar gráfico de estatísticas.\n");
        return;
    }

    // Plotar o gráfico
    plotGraph(graph);
    freeGraphInfo(graph);
}

void freeGraphInfo(GraphInfo* graph) {
    if (!graph) return;
    
    if (graph->graphTitle) free(graph->graphTitle);
    if (graph->xLabelName) free(graph->xLabelName);
    if (graph->yLabelName) free(graph->yLabelName);
    if (graph->xData) free(graph->xData);
    if (graph->yData) free(graph->yData);
    
    free(graph);
}

/*
int main() {
    // Dados de exemplo
    int x[] = {1, 2, 3, 4, 5, 6};
    int y[] = {344, 543, 433, 232, 212, 343};
    int dataSize = 6;
    
    char* graphTitle;
    char* xLabelName;
    char* yLabelName;

    graphTitle = strdup("Desempenho do Algoritmo");
    xLabelName = strdup("Tamanho da Entrada");
    yLabelName = strdup("Tempo de Execução (ms)");

    GraphInfo* graph = initializeGraphInfo(graphTitle, dataSize, xLabelName, x, yLabelName, y);
    if (!graph){
        printf("Erro ao alocar graph.\n");

        return 1;
    }

    if (!graph->xData || !graph->yData) {
        printf("Erro ao alocar dados.\n");
        free(graph);
        return 1;
    }
    
    // Plota o gráfico
    plotGraph(graph);
    
    // Limpeza
    freeGraphInfo(graph);
    
    return 0;
}
*/