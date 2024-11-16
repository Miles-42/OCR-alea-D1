# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O3 -g $(shell pkg-config --cflags gtk+-3.0 sdl2 SDL2_image)
LIBS = $(shell pkg-config --libs gtk+-3.0 sdl2 SDL2_image) -lm -rdynamic

# Source files
SOURCES = ui.c \
	../GridDetection/hough_transform.c \
	../GridDetection/local_threshold.c \
	../GridDetection/sobel.c \
	../Traitement/contrast.c \
	../Traitement/grayscale.c \
	../Traitement/noise_reduction.c \
	../utils/lines.c \
	../utils/linked_list.c \
	../utils/splitting.c \
	../utils/utils.c \
	../SudokuSolver/reader.c \
	../SudokuSolver/solver.c \
	../SudokuSolver/reassembled.c \
	../NN2/digit_network.c \
    ../NN2/utils/convert.c \
    ../NN2/result/result_network.c \
    ../NN2/XOR/matrix.c \
	../NN2/XOR/saveLoad.c \
	../NN2/XOR/maths_extra.c \
	../NN2/utils/pixel.c

# Object files (automatically generated from source files)
OBJECTS = $(SOURCES:.c=.o)

# Executable name
EXECUTABLE = my_application
DIRNAME = ../output
DIRNAME_2 = ../output/steps
DIRNAME_3 = ../output/tiles
DIRNAME_4 = ../output/reconstruction

all: $(EXECUTABLE) $(DIRNAME) $(DIRNAME_2) $(DIRNAME_3) $(DIRNAME_4)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(OBJECTS) $(LIBS)

$(DIRNAME):
	mkdir -p $(DIRNAME)

$(DIRNAME_2):
	mkdir -p $(DIRNAME_2)

$(DIRNAME_3):
	mkdir -p $(DIRNAME_3)

$(DIRNAME_4):
	mkdir -p $(DIRNAME_4)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	rm -r $(DIRNAME)/tiles/
	rm -r $(DIRNAME)/steps/
	rm -r $(DIRNAME)/reconstruction/
