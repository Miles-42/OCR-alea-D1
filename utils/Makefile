CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 $(shell pkg-config --cflags sdl2 SDL2_image) -lm
LDFLAGS =
LDLIBS = $(shell pkg-config --libs sdl2 SDL2_image) -lm

all: Rotation

SRC =  main.c utils.c
OBJ = ${SRC:.c=.o}
EXE = Rotation

Rotation: main.o utils.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean

clean:
	${RM} ${OBJ}
	${RM} ${EXE}
	${RM} test/*

# END
