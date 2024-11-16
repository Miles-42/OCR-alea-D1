# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O3 -g
LIBS = `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS = `pkg-config --libs sdl2 SDL2_image`


all: test

SRC = matrix.c main.c network.c backprop.c file_manager.c image.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

#main: main.o matrix.o network.o backprop.o file_manager.o image.o
test: image.o matrix.o


main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ) $(LIBS) $(LDFLAGS)

all : main

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END
