#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "../utils/utils.h"

/**
 * @brief Apply denoiser to the surface.
 *
 * @param surface
 */
void denoise(SDL_Surface *surface);