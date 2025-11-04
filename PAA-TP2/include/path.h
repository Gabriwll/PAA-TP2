#ifndef PATH_H
#define PATH_H

#include "dp_solver.h"
#include "problem.h"

void path_print(const PathStep *path, int path_len, const Problem *P);
void path_print_map_overlay(const PathStep *path, int path_len, const Problem *P);
int path_save_to_file(const char *outpath, const PathStep *path, int path_len);
#endif
