# Compiler
CC=gcc

# Compiler Flags
CFLAGS=-I. -Wall

# Libraries
LIBS=`pkg-config --libs sdl2 SDL2_image ` -lm

# Source files
SRCS=	digit_network.c \
		main.c\
		utils/convert.c \
		result/result_network.c \
		XOR/matrix.c \
		XOR/saveLoad.c \
		XOR/maths_extra.c \
utils/pixel.c

# Object files
OBJS=$(SRCS:.c=.o)

# Executable name
EXEC= main

# Default rule
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

# Clean rule
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean
