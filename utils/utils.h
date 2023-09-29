#pragma once

#ifndef M_PI
#  define M_PI 3.14159265358979323846
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <err.h>
#include <math.h>

/**
 * @brief Rotate the surface by the given angle.
 *
 * @param surface
 * @param x
 * @param y
 * @return Uint32
 */

void rotate(SDL_Surface *surface, double angle);

/**
 * @brief Get the Pixel object at the given coordinates.
 *
 * @param surface
 * @param x
 * @param y
 * @return Uint32
 */
Uint32 getPixel(SDL_Surface *surface, int x, int y);

/**
 * @brief Set the pixel at the given coordinates to the given color.
 *
 * @param surface
 * @param x
 * @param y
 * @param pixel
 */
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

/**
 * @brief Return a SDL Surface of the same format.
 *
 * @param surface
 * @return SDL_Surface*
 */
SDL_Surface *copySurface(SDL_Surface *surface);

/**
 * @brief Get a matrix of neighbours of a pixel.
 *
 * @param surface
 * @param x
 * @param y
 * @return Uint8*
 */
Uint8 *getNeighbours(SDL_Surface *surface, int x, int y);

/**
 * @brief Save a surface into a file.
 *
 * @param surface
 * @param filename
 */
void saveSurface(SDL_Surface *surface, char *filename);

/**
 * @brief Return the angle in radians converted in degrees.
 *
 * @param degrees
 * @return double
 */
double degreesToRadians(double degrees);
/**
 * @brief Return the angle in degrees converted in radians.
 *
 * @param radian
 * @return double
 */
double radiansToDegrees(double radian);

/**
 * @brief Return the points i,j rotated by the given angle.
 *
 * @param i
 * @param j
 * @param x
 * @param y
 * @param width
 * @param height
 * @param cos_angle
 * @param sin_angle
 */
void convertPoints(int i, int j, int *x, int *y, int width, int height,
                   double cos_angle, double sin_angle);

typedef struct SudokuCell
{
  int xTopLeft, yTopLeft;
  int xTopRight, yTopRight;
  int xBottomLeft, yBottomLeft;
  int xBottomRight, yBottomRight;
} SudokuCell;