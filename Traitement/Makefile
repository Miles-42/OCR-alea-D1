CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 $(shell pkg-config --cflags sdl2 SDL2_image) -lm
LDFLAGS =
LDLIBS = $(shell pkg-config --libs sdl2 SDL2_image) -lm


all: Constrast Grayscale Utils Noise_reduction

SRC =  contrast.c grayscale.c noise_reduction.c ../utils/utils.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=}

Constrast: constrast.o
Grayscale: grayscale.output_gray
Utils: ../utils/utils.output_gray
Noise_reduction:noise_reduction.c


.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END