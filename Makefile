# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O3 -g
LIBS = `pkg-config --cflags sdl2 SDL2_image` -lm
LDFLAGS = `pkg-config --libs sdl2 SDL2_image`

# Source files
SOURCES = main.c \
	GridDetection/hough_transform.c \
	GridDetection/local_threshold.c \
	GridDetection/sobel.c \
	Traitement/contrast.c \
	Traitement/grayscale.c \
	Traitement/noise_reduction.c \
	utils/lines.c \
	utils/linked_list.c \
	utils/splitting.c \
	utils/utils.c

# Object files (automatically generated from source files)
OBJECTS = $(SOURCES:.c=.o)

# Executable name
EXECUTABLE = main_program

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
