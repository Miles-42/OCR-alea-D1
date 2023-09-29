#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "../utils/lines.h"
#include "../utils/linked_list.h"
#include "../utils/utils.h"

/**
 * @brief Apply hough-transform to the surface.
 *
 * @param surface
 * @param list
 */
void houghTransform(SDL_Surface *surface, List *list);