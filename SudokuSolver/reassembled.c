#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "reassembled.h"

void case_grid(SDL_Surface *dest, int x, int y, int number, size_t isSolved)
{
    char out[200];

    if (isSolved == 1)
    {
        snprintf(out, sizeof(out), "Reconstruction/numbers_modif/%d.jpg", number);
    }
    else
    {
        snprintf(out, sizeof(out), "Reconstruction/numbers/%d.jpg", number);
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

void case_grid_random(SDL_Surface *dest, int x, int y, int number, size_t isSolved)
{
    char out[200];
    int r = rand() % 100 + 1;

    if (isSolved == 1)
    {
        if (r % 2 == 0)
        {
            return;
        }

        else
        {
            snprintf(out, sizeof(out), "Reconstruction/numbers_modif/%d.jpg", number);
        }
    }

    else
    {
        snprintf(out, sizeof(out), "Reconstruction/numbers/%d.jpg", number);
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

void reconstruction_image(int grid_solved[9][9], int grid_not[9][9])
{
    char *grille_vierge = "Reconstruction/grid_empty.jpg";
    char *output_grid = "Reconstruction/grid_final.jpg";

    SDL_Surface *yessir = IMG_Load(grille_vierge);
    SDL_Surface *grille = SDL_ConvertSurfaceFormat(yessir, SDL_PIXELFORMAT_RGB888, 0);
    SDL_FreeSurface(yessir);

    int weight = grille->w / 9;
    int height = grille->h / 9;

    int dh = 4;
    int dw = 4;

    int i;
    int j;

    size_t isOK = 0;

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (grid_solved[i][j] == grid_not[i][j])
            {
                isOK = 0;
            }

            else
            {
                isOK = 1;
            }

            case_grid_random(grille, dw, dh, grid_solved[i][j], isOK);
            dw += weight;
        }

        dw = 4;
        dh += height;
    }

    IMG_SaveJPG(grille, output_grid, 90);
    SDL_FreeSurface(grille);
}
