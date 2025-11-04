#include <stdio.h>
#include "utils.h"

void print_stats(const SolverStats *s) {
    if (!s) return;
    printf("=== Estatísticas do solver ===\n");
    printf("Nós expandidos: %ld\n", s->nodes_expanded);
    printf("Estados alcançáveis: %ld\n", s->reachable_states);
    printf("Maior fronteira observada (col por col): %ld\n", s->max_frontier);
    printf("Tempo aproximado: %.3f ms\n", s->approx_ms);
    printf("=============================\n");
}
