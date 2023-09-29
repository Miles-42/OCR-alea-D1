#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include "../utils/linked_list.h"
#include "../utils/utils.h"

typedef struct Line
{
  int x1, y1, x2, y2;
} Line;

void lineCleaning(List *list);

void drawLine(SDL_Surface *surface, int xStart, int yStart, int xEnd, int yEnd,
              SDL_Color color, int width);

void drawLines(SDL_Surface *surface, List *lines, SDL_Color color, int width);

double line_angle(Line *l1, Line *l2);
double getAngle(Line *l);
double min(double a, double b);
double getRotationAngle(List *lines);

void rotateLines(List *lines, double angle, int width, int height);
void drawSquare(SDL_Surface *surface, SudokuCell *cell, SDL_Color color,
                int width);
void getIntersection(Line l1, Line l2, int *x, int *y, int width, int height);
SudokuCell selectSudoku(SDL_Surface *surface, List *squares);
SudokuCell makeSquare(int x1, int y1, int x2, int y2, int x3, int y3, int x4,
                      int y4);
SudokuCell selectLeftMostCell(List *squares);
SudokuCell selectRightMostCell(List *squares);

List *squareDetection(List *lines, int width, int height);
List *squareFilter(List *squares);

void drawSquares(SDL_Surface *surface, List *squares, SDL_Color color);
void point(SDL_Surface *surface, int x, int y, Uint32 pixel);
void highlight(SDL_Surface *surface, List *lines);