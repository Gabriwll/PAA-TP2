
TARGET = trabalho.exe

CC = gcc
CFLAGS = -Wall -O2 -std=c99

INCLUDES = -Iinclude \
           -IC:/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/include

LIBS = -LC:/SDL3/SDL3-devel-3.2.26-mingw/SDL3-3.2.26/x86_64-w64-mingw32/lib \
       -lmingw32 -lSDL3


SRC = src/main.c \
      src/problem.c \
      src/dp_solver.c \
      src/path.c \
      src/utils.c \
      src/gui.c \
      src/generator.c

all:
	@echo 🔧 Compilando o projeto...
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) $(LIBS) -o $(TARGET)
	@echo ✅ Compilação concluída com sucesso!

run: all
	@echo ▶️ Executando exemplo...
	./$(TARGET) exemplos/mapa_5x5.txt -v

clean:
	@echo 🧹 Limpando arquivos...
	del $(TARGET) 2>nul || true
