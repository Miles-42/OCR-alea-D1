#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "../utils/utils.h"

void invert(SDL_Surface *surface);

/**
 * @brief Add contrast to the surface.
 *
 * @param surface
 */
void contrast(SDL_Surface *surface);