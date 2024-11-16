#include <stdio.h>
#include <err.h>
#include <SDL2/SDL_image.h>

#include "reader.h"
#include "solver.h"
#include "writer.h"
#include "reassembled.h"

#define SIZE 9

void print(int arr[9][9]);
void copy_array(int grid_src[SIZE][SIZE], int grid_dest[SIZE][SIZE]);

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    errx(1, "Please input the right argument");
  }

  int grid[SIZE][SIZE] = {0};

  reader(argv[1], grid);

  int grid_actu[SIZE][SIZE];
  copy_array(grid, grid_actu);
  solveSudoku(grid, 0, 0);

  writer(argv[1], grid);
  // reconstruction_image(grid, grid_actu);
}

void print(int arr[9][9])
{
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
      printf("%d", arr[i][j]);
    printf("\n");
  }
}

void copy_array(int grid_src[SIZE][SIZE], int grid_dest[SIZE][SIZE])
{
  for (size_t i = 0; i < SIZE; i++)
  {
    for (size_t j = 0; j < SIZE; j++)
    {
      grid_dest[i][j] = grid_src[i][j];
    }
  }
}
