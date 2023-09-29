#include "main.h"

int main(int argv, char **argc)
{
  // Checking the number of arguments for correct usage
  if (argv != 2)
  {
    fprintf(stderr, "usage: %s image\n", argc[0]);
    return 1;
  }

  // Load the input image
  SDL_Surface *surface = IMG_Load(argc[1]);
  if (surface == NULL)
  {
    fprintf(stderr, "IMG_Load: %s\n", SDL_GetError());
    return 1;
  }

  // Convert the image format to 32bits
  surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
  SDL_Surface *copy = copySurface(surface);

  printf("🧵Launching multi-thread image processing\n");
  printf("----- \n");

  printf("📸 Converting & applying to grayscale...\n");
  grayscale(copy);
  saveSurface(copy, "output/steps/1-grayscale.jpg");

  printf("🖌️ Applying contrast...\n");
  contrast(copy);
  saveSurface(copy, "output/steps/2-contrast.jpg");

  printf("🔊 Applying noise detection & reduction...\n");
  denoise(copy);
  saveSurface(copy, "output/steps/3-denoise.jpg");

  printf("✨ Applying local threshold...\n");
  localThreshold(copy);
  saveSurface(copy, "output/steps/4-local_threshold.jpg");

  SDL_Surface *surfaceToSplit = copySurface(copy);
  invert(surfaceToSplit);

  printf("💽 Applying Sobel operator...\n");
  sobelEdgeDetection(copy);
  saveSurface(copy, "output/steps/5-sobel.jpg");

  printf("📐Applying Hough transform algorithm...\n");
  List *lines = initList();
  houghTransform(copy, lines);
  lineCleaning(lines);
  printf("        📈 Number of detected lines: %d\n", listLength(lines));

  SDL_Color black = {0, 0, 0, 255};
  drawLines(surfaceToSplit, lines, black, 15);

  SDL_Color color = {255, 0, 0, 255};
  SDL_Surface *surfacelines = copySurface(copy);
  drawLines(surfacelines, lines->next, color, 1);
  saveSurface(surfacelines, "output/steps/6-lines.jpg");

  double angle = getRotationAngle(lines);
  printf("        ⭕ Found angle : %2f\n", radiansToDegrees(angle));

  if (angle > 0.1 || angle < -0.1)
  {
    printf("- Rotating the image...\n");
    rotate(copy, angle);
    rotate(surfaceToSplit, angle);
    lines = initList();
    houghTransform(copy, lines);
    lineCleaning(lines);
  }
  saveSurface(copy, "output/steps/8-rotate.jpg");
  /*Bug qui clc*/
  printf("🟦 Applying square detection...\n");

  List *squares = squareDetection(lines, surface->w, surface->h);
  printf("        📈 Number of detected squares: %d\n", listLength(squares));
  squares = squareFilter(squares);
  printf("        📈 Number of filtered squares: %d\n", listLength(squares));

  SDL_Surface *drawSquareSurface = copySurface(copy);
  drawSquares(drawSquareSurface, squares, color);
  saveSurface(drawSquareSurface, "output/steps/9-draw_squares.jpg");

  SudokuCell sudokuCell = selectSudoku(drawSquareSurface, squares);
  saveSurface(drawSquareSurface, "output/steps/10-draw_sudoku.jpg");

  int distX = sudokuCell.xTopRight - sudokuCell.xBottomLeft;
  int distY = sudokuCell.yTopRight - sudokuCell.yBottomLeft;
  printf("✂️ Splitting the image into tiles...\n");
  splitImage(surfaceToSplit, sudokuCell.xBottomLeft, sudokuCell.yBottomLeft, distX, distY);
  printf("🧵 Image processing finished\n");
  printf("----- \n");

  // Clean up and free surfaces
  SDL_FreeSurface(surface);
  SDL_FreeSurface(copy);
  SDL_FreeSurface(surfaceToSplit);
  SDL_FreeSurface(surfacelines);
  SDL_FreeSurface(drawSquareSurface);

  return 0;
}
