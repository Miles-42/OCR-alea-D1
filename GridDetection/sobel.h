#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "../utils/utils.h"

/**
 * @brief Apply sobel edge detection to the surface.
 *
 * @param surface
 */
void sobelEdgeDetection(SDL_Surface *surface);