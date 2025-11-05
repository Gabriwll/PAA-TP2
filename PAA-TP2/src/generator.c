#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "problem.h"
#include "generator.h"

void generator_run(void) {
    int w, h, num_anchors, num_monsters, num_blocks;
    char basename[256];
    unsigned int seed;

    printf("=== Gerador de Mapas ===\n");

    printf("Largura do mapa: ");
    if (scanf("%d", &w) != 1) { fprintf(stderr, "Entrada inválida\n"); return; }

    printf("Altura do mapa: ");
    if (scanf("%d", &h) != 1) { fprintf(stderr, "Entrada inválida\n"); return; }

    printf("Número de âncoras temporais: ");
    if (scanf("%d", &num_anchors) != 1) { fprintf(stderr, "Entrada inválida\n"); return; }

    printf("Número de monstros: ");
    if (scanf("%d", &num_monsters) != 1) { fprintf(stderr, "Entrada inválida\n"); return; }

    printf("Número de blocos: ");
    if (scanf("%d", &num_blocks) != 1) { fprintf(stderr, "Entrada inválida\n"); return; }

    printf("Nome base do arquivo de saída (sem .txt): ");
    if (scanf("%s", basename) != 1) { fprintf(stderr, "Entrada inválida\n"); return; }

    printf("Semente (0 para aleatória): ");
    if (scanf("%u", &seed) != 1) { fprintf(stderr, "Entrada inválida\n"); return; }
    if (seed == 0) seed = (unsigned int)time(NULL);

    srand(seed);

    Problem P;
    P.h = h;
    P.w = w;
    P.F_init = 100;
    P.D = 3;
    P.N = 60;

    // Aloca os dois mapas
    for (int m = 0; m < 2; ++m) {
        P.map[m] = malloc(h * sizeof(Cell*));
        for (int i = 0; i < h; ++i) {
            P.map[m][i] = malloc(w * sizeof(Cell));
            for (int j = 0; j < w; ++j) {
                P.map[m][i][j].type = CELL_EMPTY;
                P.map[m][i][j].val = 0;
                sprintf(P.map[m][i][j].raw, "000");
            }
        }
    }

    // Distribui blocos
    for (int b = 0; b < num_blocks; ++b) {
        int m = rand() % 2, i = rand() % h, j = rand() % w;
        P.map[m][i][j].type = CELL_BLOCK;
        sprintf(P.map[m][i][j].raw, "***");
    }

    // Distribui âncoras
    for (int a = 0; a < num_anchors; ++a) {
        int m = rand() % 2, i = rand() % h, j = rand() % w;
        P.map[m][i][j].type = CELL_ANCHOR;
        sprintf(P.map[m][i][j].raw, "AAA");
    }

    // Distribui monstros
    for (int mo = 0; mo < num_monsters; ++mo) {
        int m = rand() % 2, i = rand() % h, j = rand() % w;
        P.map[m][i][j].type = CELL_MONSTER;
        int val = rand() % 20 + 5;
        P.map[m][i][j].val = val;
        sprintf(P.map[m][i][j].raw, "%03d", val);
    }

    // Salva o arquivo
    char filename[300];
    snprintf(filename, sizeof(filename), "%s.txt", basename);
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Não foi possível criar o arquivo\n");
        return;
    }

    fprintf(fp, "%d %d %d %d %d\n", P.h, P.w, P.F_init, P.D, P.N);
    for (int m = 0; m < 2; ++m) {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                fprintf(fp, "%s ", P.map[m][i][j].raw);
            }
            fprintf(fp, "\n");
        }
    }
    fclose(fp);

    printf("Arquivo '%s' gerado com sucesso! (seed=%u)\n", filename, seed);

    // Libera memória
    for (int m = 0; m < 2; ++m) {
        for (int i = 0; i < h; ++i)
            free(P.map[m][i]);
        free(P.map[m]);
    }
}
