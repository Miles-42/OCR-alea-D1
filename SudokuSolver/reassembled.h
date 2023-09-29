#pragma once

void case_grid(SDL_Surface *dest, int x, int y, int number, size_t isSolved);
void reconstruction_image(int grid_solved[9][9], int grid_not[9][9]);