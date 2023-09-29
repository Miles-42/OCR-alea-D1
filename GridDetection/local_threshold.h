#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "../utils/utils.h"

/**
 * @brief Apply adaptive threshold to the surface.
 *
 * @param surface
 */
void localThreshold(SDL_Surface *surface);