#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "path.h"
#include "problem.h"

void path_print(const PathStep *path, int path_len, const Problem *P) {
    if (!path || path_len==0) {
        printf("A calamidade de Nikador é inevitável\n");
        return;
    }
    for (int i=0;i<path_len;++i) {
        printf("%d %d\n", path[i].row, path[i].col);
    }
    int final_force = path[path_len-1].force;
    if (final_force >= P->N) {
        printf("A ruína de Nikador é iminente\n");
    } else {
        printf("Será necessário mais planejamento para parar a calamidade\n");
    }
}

void path_print_map_overlay(const PathStep *path, int path_len, const Problem *P) {
    int h = P->h, w = P->w;
    /* build character map for present (map 0) and past (map 1) separately */
    for (int m=0;m<2;++m) {
        printf("Mapa %s:\n", m==0 ? "Presente" : "Passado");
        for (int i=0;i<h;++i) {
            for (int j=0;j<w;++j) {
                char buf[8];
                const char *cell_s = P->map[m][i][j].raw;
                int marked = 0;
                for (int k=0;k<path_len;++k) {
                    if (path[k].map == m && path[k].row == i && path[k].col == j) { marked = 1; break; }
                }
                if (marked) printf("X ");
                else printf("%s ", cell_s);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int path_save_to_file(const char *outpath, const PathStep *path, int path_len) {
    FILE *fp = fopen(outpath, "w");
    if (!fp) return -1;
    fprintf(fp, "row,col,map,force\n");
    for (int i=0;i<path_len;++i) {
        fprintf(fp, "%d,%d,%d,%d\n", path[i].row, path[i].col, path[i].map, path[i].force);
    }
    fclose(fp);
    return 0;
}
