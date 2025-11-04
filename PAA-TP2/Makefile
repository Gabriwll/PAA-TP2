CC = gcc
CFLAGS = -Wall -O2 -std=c99 -Iinclude

SRC = src/main.c src/problem.c src/dp_solver.c src/path.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = trabalho

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
