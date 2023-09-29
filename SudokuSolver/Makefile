# Makefile

TARGET = solver
CC = gcc -fsanitize=address
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image`

OBJ = reader.o solver.o writer.o main.o reassembled.o
DEP = ${OBJ:.o=.d}

all: solver

solver: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} ${TARGET} grid_00.result grid_01.result

-include ${DEP}

# END