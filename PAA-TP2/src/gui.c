#include "SDL3/SDL.h"
#include "problem.h"
#include "path.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define CELL_SIZE 40
#define DELAY 150

void gui_show_path(const Problem *P, const PathStep *path, int path_len) {
    if (!P || !path || path_len <= 0) {
        fprintf(stderr, "Erro: parâmetros inválidos para gui_show_path\n");
        return;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Falha ao inicializar SDL: %s\n", SDL_GetError());
        return;
    }

    int w = P->w, h = P->h;
    int margin = 50;
    int win_w = (w * CELL_SIZE * 2) + margin * 3;
    int win_h = h * CELL_SIZE + 100;

    SDL_Window *win = SDL_CreateWindow("Melhor Caminho até Nikador", win_w, win_h, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, NULL);

    if (!renderer) {
        fprintf(stderr, "Falha ao criar renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return;
    }

    printf("\n=== LEGENDA DE CORES ===\n");
    printf("Verde claro : Caminho percorrido\n");
    printf("Amarelo     : Posição atual\n");
    printf("Verde       : Área de descanso\n");
    printf("Laranja     : Inimigo\n");
    printf("Azul        : Âncora temporal\n");
    printf("Vermelho    : Área intransponível\n");
    printf("=========================\n\n");

    bool quit = false;
    SDL_Event e;
    int current = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = true;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) quit = true;
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        for (int map = 0; map < 2; map++) {
            int offset_x = margin + map * (w * CELL_SIZE + margin);
            for (int i = 0; i < h; i++) {
                for (int j = 0; j < w; j++) {
                    Cell cell = P->map[map][i][j];
                    SDL_FRect rect = {offset_x + j * CELL_SIZE, 60 + i * CELL_SIZE, CELL_SIZE - 2, CELL_SIZE - 2};

                    if (strncmp(cell.raw, "###", 3) == 0)
                        SDL_SetRenderDrawColor(renderer, 180, 50, 50, 255);
                    else if (strncmp(cell.raw, "AAA", 3) == 0)
                        SDL_SetRenderDrawColor(renderer, 50, 100, 220, 255);
                    else if (strncmp(cell.raw, "000", 3) == 0)
                        SDL_SetRenderDrawColor(renderer, 80, 200, 80, 255);
                    else
                        SDL_SetRenderDrawColor(renderer, 220, 140, 40, 255);

                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        for (int i = 0; i <= current && i < path_len; i++) {
            PathStep step = path[i];
            int offset_x = margin + step.map * (w * CELL_SIZE + margin);
            SDL_FRect rect = {offset_x + step.col * CELL_SIZE, 60 + step.row * CELL_SIZE, CELL_SIZE - 2, CELL_SIZE - 2};
            
            if (i == current)
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 50, 255, 50, 200);
            
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(DELAY);

        if (current < path_len - 1) current++;
        else SDL_Delay(1000);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
