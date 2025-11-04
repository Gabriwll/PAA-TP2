#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "problem.h"
#include "dp_solver.h"
#include "path.h"
#include "utils.h"

/* Simple command line:
   ./trabalho <input1> [<input2> ...] [-v] [-o outpath] [-S statsfile]
*/

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <input1> [<input2> ...] [-v] [-o outpath] [-S statsfile]\n", argv[0]);
        return 1;
    }
    int verbose = 0;
    const char *outpath = NULL;
    const char *statsfile = NULL;

    /* collect input files (non-flag args) */
    char **inputs = malloc(argc * sizeof(char*));
    int n_inputs = 0;
    for (int i=1;i<argc;++i) {
        if (strcmp(argv[i],"-v")==0 || strcmp(argv[i],"--verbose")==0) { verbose=1; continue; }
        if (strcmp(argv[i],"-o")==0 && i+1<argc) { outpath = argv[++i]; continue; }
        if (strcmp(argv[i],"-S")==0 && i+1<argc) { statsfile = argv[++i]; continue; }
        if (argv[i][0] == '-') { fprintf(stderr, "Flag desconhecida: %s\n", argv[i]); return 1; }
        inputs[n_inputs++] = argv[i];
    }
    if (n_inputs == 0) { fprintf(stderr, "Nenhum ficheiro de entrada indicado\n"); return 1; }

    for (int idx=0; idx<n_inputs; ++idx) {
        const char *infile = inputs[idx];
        Problem *P = problem_read_from_file(infile);
        if (!P) {
            fprintf(stderr, "Erro ao ler arquivo: %s\n", infile);
            continue;
        }
        if (verbose) printf("Processando %s (h=%d w=%d F=%d D=%d N=%d)\n",
                            infile, P->h, P->w, P->F_init, P->D, P->N);

        SolverStats stats = {0};
        int path_len = 0;
        PathStep *path = solve_problem_dp(P, &path_len, &stats, verbose);

        if (path == NULL || path_len == 0) {
            printf("A calamidade de Nikador é inevitável\n");
        } else {
            path_print(path, path_len, P);
            if (verbose) {
                print_stats(&stats);
                path_print_map_overlay(path, path_len, P);
            }
            if (outpath) {
                if (path_save_to_file(outpath, path, path_len) != 0)
                    fprintf(stderr, "Falha ao salvar caminho em %s\n", outpath);
                else if (verbose) printf("Caminho salvo em %s\n", outpath);
            }
            if (statsfile) {
                FILE *sf = fopen(statsfile, "w");
                if (sf) {
                    fprintf(sf, "nodes_expanded,%ld\nreachable_states,%ld\nmax_frontier,%ld\nms,%.3f\n",
                            stats.nodes_expanded, stats.reachable_states, stats.max_frontier, stats.approx_ms);
                    fclose(sf);
                } else {
                    fprintf(stderr, "Nao foi possivel escrever stats em %s\n", statsfile);
                }
            }
        }

        free_pathsteps(path);
        problem_free(P);
    }

    free(inputs);
    return 0;
}
