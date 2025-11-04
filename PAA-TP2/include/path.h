#ifndef PATH_H
#define PATH_H

#include "dp_solver.h"
#include "problem.h"

/* print path to stdout */
void path_print(const PathStep *path, int path_len, const Problem *P);

/* print map with path marked (path marker 'X') */
void path_print_map_overlay(const PathStep *path, int path_len, const Problem *P);

/* save path to file (csv-like): row,col,map,force */
int path_save_to_file(const char *outpath, const PathStep *path, int path_len);

#endif /* PATH_H */
