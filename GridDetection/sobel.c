#include "sobel.h"

Uint8 getRed(Uint32 color)
{
  return (color >> 24) & 0xFF;
}

int convX(int x, int y, SDL_Surface *surface)
{
  // Sobel masks for X axis
  int sum = getRed(getPixel(surface, x - 1, y - 1)) * -1;
  sum += getRed(getPixel(surface, x - 1, y)) * -2;
  sum += getRed(getPixel(surface, x - 1, y + 1)) * -1;
  sum += getRed(getPixel(surface, x + 1, y - 1)) * 1;
  sum += getRed(getPixel(surface, x + 1, y)) * 2;
  sum += getRed(getPixel(surface, x + 1, y + 1)) * 1;
  return sum;
}

int convY(int x, int y, SDL_Surface *surface)
{
  // Sobel masks for Y axis
  int sum = getRed(getPixel(surface, x - 1, y - 1)) * -1;
  sum += getRed(getPixel(surface, x, y - 1)) * -2;
  sum += getRed(getPixel(surface, x + 1, y - 1)) * -1;
  sum += getRed(getPixel(surface, x - 1, y + 1)) * 1;
  sum += getRed(getPixel(surface, x, y + 1)) * 2;
  sum += getRed(getPixel(surface, x + 1, y + 1)) * 1;
  return sum;
}

void __invert(SDL_Surface *surface)
{
  Uint8  r, g, b;
  Uint32 pixel;

  for (int y = 0; y < surface->h; y++)
  {
    for (int x = 0; x < surface->w; x++)
    {
      pixel = getPixel(surface, x, y);
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      r     = 255 - r;
      g     = 255 - g;
      b     = 255 - b;
      pixel = SDL_MapRGB(surface->format, r, g, b);
      putPixel(surface, x, y, pixel);
    }
  }
}

void sobelEdgeDetection(SDL_Surface *surface)
{
  // Use Sobel edge detection algorithm
  // https://en.wikipedia.org/wiki/Sobel_operator
  // https://fiveko.com/sobel-filter

  __invert(surface);
  SDL_Surface *copy = copySurface(surface);
  // convert copy to RGBA
  copy = SDL_ConvertSurfaceFormat(copy, SDL_PIXELFORMAT_RGBA8888, 0);
  for (int y = 1; y < surface->h - 1; y++)
  {
    for (int x = 1; x < surface->w - 1; x++)
    {
      int gx = convX(x, y, copy);
      int gy = convY(x, y, copy);
      int g  = abs(gx) + abs(gy);
      // Binarize the Image (black and white)
      Uint32 pixel = SDL_MapRGB(copy->format, g, g, g);
      putPixel(surface, x, y, (pixel > 0x00FFFFFF) ? 0xFFFFFFFF : 0x00000000);
    }
  }
  SDL_FreeSurface(copy);
}