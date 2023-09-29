#include "utils.h"

void rotate(SDL_Surface *surface, double angle)
{
  SDL_Surface *copy      = copySurface(surface);
  double       cos_angle = cos(angle);
  double       sin_angle = sin(angle);
  SDL_FillRect(surface, NULL, 0);
  for (int i = 0; i < surface->w; i++)
  {
    for (int j = 0; j < surface->h; j++)
    {
      // Get the coordinates of the pixel in the copy surface
      int x, y;
      convertPoints(j, i, &y, &x, surface->w, surface->h, cos_angle,
                    sin_angle);

      // If the new coordinates are inside the surface, put the pixel
      if (x >= 0 && x < surface->w && y >= 0 && y < surface->h)
      {
        Uint32 pixel = getPixel(copy, x, y);
        putPixel(surface, i, j, pixel);
      }
    }
  }
  SDL_FreeSurface(copy);
}

void convertPoints(int i, int j, int *x, int *y, int width, int height,
                   double cos_angle, double sin_angle)
{
  *x = (i - width / 2) * cos_angle + (j - height / 2) * sin_angle + width / 2;
  *y = -(i - width / 2) * sin_angle + (j - height / 2) * cos_angle
       + height / 2;
}

Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
  // Get the pixel using the row major order
  Uint32 *pixels = (Uint32 *)surface->pixels;
  return pixels[y * surface->w + x];
}

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  // Our pixels are on 32bits in SDL_PIXELFORMAT_RGB888
  Uint32 *pixels             = surface->pixels;
  pixels[y * surface->w + x] = pixel;
}

SDL_Surface *copySurface(SDL_Surface *surface)
{
  Uint32 *newPixels = malloc((size_t)surface->pitch * surface->h);
  if (newPixels == NULL)
  {
    errx(1, "malloc");
  }
  memcpy(newPixels, surface->pixels, (size_t)surface->pitch * surface->h);
  SDL_Surface *copy = SDL_CreateRGBSurfaceWithFormatFrom(
      newPixels, surface->w, surface->h, 32, surface->pitch,
      surface->format->format);
  return copy;
}

Uint8 *getNeighbours(SDL_Surface *surface, int x, int y)
{
  int    range = 3;
  Uint8 *ret   = malloc((9 + 1) * sizeof(Uint8));
  if (ret == NULL)
  {
    errx(1, "malloc");
  }
  int   count  = 0;
  int   width  = surface->w;
  int   height = surface->h;
  Uint8 r, g, b;
  // Initialize the array to 0
  for (int i = 0; i < 9; i++)
  {
    ret[i] = 0;
  }
  // Get the neighbours up to a range around the pixel
  for (int i = -range; i <= range; i++)
  {
    for (int j = range; j <= range; j++)
    {
      // Check if the pixel is inside the surface
      if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height)
      {
        Uint32 color = getPixel(surface, x + i, y + j);
        SDL_GetRGB(color, surface->format, &r, &g, &b);
        ret[count] = r;
        count++;
      }
      else
      {
        ret[count] = 255;
        count++;
      }
    }
  }
  return ret;
}

void saveSurface(SDL_Surface *surface, char *filename)
{
  IMG_SaveJPG(surface, filename, 100);
}

double degreesToRadians(double degrees)
{
  return degrees * M_PI / 180.0;
}
double radiansToDegrees(double radian)
{
  return radian * 180.0 / M_PI;
}
