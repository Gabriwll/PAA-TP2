# ============================================================
# Makefile - Projeto PAA-TP2 (SDL3 + MinGW)
# ============================================================

CC = gcc
CFLAGS = -Wall -O2 -std=c99 -Iinclude \
         -IC:/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/include \
         -IC:/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/include/SDL3

SDL_LIB_DIR = C:/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib

SRC = src/main.c src/problem.c src/dp_solver.c src/path.c src/utils.c src/gui.c src/generator.c
OBJ = $(SRC:.c=.o)
TARGET = trabalho.exe

# 🔗 SDL3 e dependências do Windows (na ordem correta)
LIBS = -Wl,-Bstatic $(SDL_LIB_DIR)/libSDL3.dll.a -Wl,-Bdynamic \
       -lmingw32 -lwinmm -ldxguid -limm32 -lole32 -loleaut32 -lversion -luser32 -lgdi32

# ============================================================
# Targets
# ============================================================

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "🔗 Compilando e linkando com SDL3..."
	$(CC) $(CFLAGS) $(OBJ) -L$(SDL_LIB_DIR) -o $(TARGET) $(LIBS)

clean:
	@echo "🧹 Limpando objetos e executáveis..."
	del /Q src\*.o 2>nul || true
	del /Q $(TARGET) 2>nul || true

run: $(TARGET)
	./$(TARGET)