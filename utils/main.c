#include <stdlib.h>
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils.h"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        errx(1, "Usage: image.jpg <angle>");
    }

    double angle = atof(argv[2]);

    SDL_Surface *surface = IMG_Load(argv[1]);
    surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
    SDL_Surface *copy = copySurface(surface);

    rotate(copy, angle);
    saveSurface(copy, "test/rotate_img.jpg");

    SDL_FreeSurface(copy);
    SDL_FreeSurface(surface);

    SDL_Quit();

    return EXIT_SUCCESS;
}