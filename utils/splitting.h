#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Splits the image
 * @param sfcSudoku The surface of the sudoku image
 * @param orgX The x value of the left top most corner on the grid
 * @param orgY The y value of the left top most corner on the grid
 * @param w The width of the image
 * @param h The height of the image
 */
void splitImage(SDL_Surface *sfcSudoku, int orgX, int orgY, int w, int h);

/**
 * @brief The main function for the image splitting
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit code
 */
int splitMain(int argc, char **argv);
