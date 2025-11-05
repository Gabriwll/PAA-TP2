# ============================================================
# Makefile - Projeto PAA-TP2
# Compila o programa principal (trabalho.exe) e o gerador (generator.exe)
# ============================================================

# Compilador e flags
CC = gcc
CFLAGS = -Wall -O2 -std=c99 -Iinclude \
         -IC:/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/include
LDFLAGS = -LC:/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib
LIBS = -lmingw32 -lSDL3

# Fontes e objetos
SRC_DIR = src
SRC_MAIN = $(SRC_DIR)/main.c $(SRC_DIR)/problem.c $(SRC_DIR)/dp_solver.c \
           $(SRC_DIR)/path.c $(SRC_DIR)/utils.c $(SRC_DIR)/gui.c $(SRC_DIR)/generator.c
OBJ_MAIN = $(SRC_MAIN:.c=.o)

# Executáveis
TARGET = trabalho.exe
GENERATOR = generator.exe

# ============================================================
# Targets
# ============================================================

all: $(TARGET) $(GENERATOR)

$(TARGET): $(OBJ_MAIN)
	$(CC) $(CFLAGS) $(OBJ_MAIN) $(LDFLAGS) $(LIBS) -o $(TARGET)

$(GENERATOR): $(SRC_DIR)/generator.c include/generator.h
	$(CC) $(CFLAGS) $(SRC_DIR)/generator.c $(LDFLAGS) $(LIBS) -o $(GENERATOR)

# Executar o programa principal
run: $(TARGET)
	./$(TARGET) exemplos/exemplo.txt -v

# Executar o gerador (extra4)
generator: $(GENERATOR)
	./$(GENERATOR)

# Limpar objetos e binários
clean:
	del /Q $(SRC_DIR)\*.o 2>nul || true
	del /Q $(TARGET) $(GENERATOR) 2>nul || true

# ============================================================
# Fim do Makefile
# ============================================================
