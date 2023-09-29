#include "splitting.h"

int max(int a, int b)
{
  return a > b ? a : b;
}

SDL_Surface *resizeTo(SDL_Surface *surface, int w, int h)
{
  SDL_Surface *resized = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
  SDL_BlitScaled(surface, NULL, resized, NULL);
  return resized;
}

void splitImage(SDL_Surface *sfcSudoku, int orgX, int orgY, int w, int h)
{
  // Define the size of each sudoku tile
  int tileX = w / 9, tileY = h / 9;
  int sizeTile = max(tileX, tileY);

  // Loop through each sudoku tiles
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      // Create a new surface for the current tile
      SDL_Surface *sfcTile
          = SDL_CreateRGBSurface(0, sizeTile, sizeTile, 32, 0, 0, 0, 0);
      // set surface to mode RGBA_8888
      sfcTile = SDL_ConvertSurfaceFormat(sfcTile, SDL_PIXELFORMAT_RGBA8888, 0);
      // Create the rectangle at the correct coordinates
      SDL_Rect currTile;
      int      threshold = 5;

      currTile.x = j * sizeTile + orgX + threshold;
      currTile.y = i * sizeTile + orgY;
      currTile.w = sizeTile;
      currTile.h = sizeTile;

      // Copy the tile from the sudoku surface into the tile surface juste //
      // created
      SDL_BlitSurface(sfcSudoku, &currTile, sfcTile, NULL);

      // Save the tile to a new .BMP file
      char path[50]   = "output/tiles/";
      char filename[] = {i + '0', j + '0', '.', 'j', 'p', 'g', 0};
      strcat(path, filename);

      SDL_Surface *resizedSurface = resizeTo(sfcTile, 28, 28);
      resizedSurface              = SDL_ConvertSurfaceFormat(resizedSurface,
                                                             SDL_PIXELFORMAT_RGBA8888, 0);
      IMG_SaveJPG(resizedSurface, path, 100);

      // Free the surface created for the current tile
      SDL_FreeSurface(sfcTile);
    }
  }
}

int splitMain(int argc, char **argv)
{
  // Checks the number of arguments.
  if (argc != 2)
    errx(EXIT_FAILURE, "Usage: sudoku-number");

  char *sudokuPath = argv[1];

  char  target[10]      = "sudoku";
  char *sudokuNumberPos = strstr(sudokuPath, target);
  if (sudokuNumberPos == NULL)
    errx(EXIT_FAILURE, "Could not get sudoku number");

  char *sudokuNumber = sudokuNumberPos + strlen(target);

  if (sudokuNumber == NULL)
    printf("Sudoku number: %s", sudokuNumber);
  int ORGX, ORGY, W, H;
  switch (sudokuNumber[0])
  {
    case '1':
      ORGX = 0, ORGY = 0, W = 1000, H = 1000; // sudoku1
      break;
    case '2':
      ORGX = 342, ORGY = 216, W = 830, H = 821; // sudoku2
      break;
    case '3':
      ORGX = 130, ORGY = 89, W = 520, H = 520; // sudoku3
      break;
    case '4':
      ORGX = 415, ORGY = 162, W = 1132, H = 1147; // sudoku4
      break;
    default:
      errx(EXIT_FAILURE, "Usage: sudoku-number");
  }
  // char *path = "images/sudoku";
  // strcat(path, argv[1]);
  // strcat(path, ".jpeg");

  // Create a surface to split from
  SDL_Surface *surface = IMG_Load(sudokuPath);
  if (surface == NULL)
    errx(EXIT_FAILURE, "%s", SDL_GetError());

  // Split the image into 81 tiles to feed to the AI
  splitImage(surface, ORGX, ORGY, W, H);

  // Free the main surface
  SDL_FreeSurface(surface);

  return EXIT_SUCCESS;
}
