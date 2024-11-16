#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "reassembled.h"

void case_grid(SDL_Surface *dest, int x, int y, int number, size_t isSolved)
{
    char out[200];

    if (isSolved == 1)
    {
        snprintf(out, sizeof(out), "../SudokuSolver/Reconstruction/numbers_modif/%i.jpg", number);
    }
    else
    {
        snprintf(out, sizeof(out), "../SudokuSolver/Reconstruction/numbers/%i.jpg", number);
    }

    SDL_Surface *img = IMG_Load(out);
    SDL_Surface *src = SDL_ConvertSurfaceFormat(img, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(img);

    SDL_Rect rect_src;
    rect_src.x = 0;
    rect_src.y = 0;
    rect_src.w = 50;
    rect_src.h = 50;

    SDL_Rect rect_dest;
    rect_dest.x = x;
    rect_dest.y = y;
    rect_dest.w = 50;
    rect_dest.h = 50;

    SDL_BlitSurface(src, &rect_src, dest, &rect_dest);
    SDL_FreeSurface(src);
}