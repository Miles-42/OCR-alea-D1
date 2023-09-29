#include "contrast.h"

void invert(SDL_Surface *surface)
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

Uint8 maxColor(SDL_Surface *surface)
{
  // Get the max (red) color value in the image
  Uint8 max = 0;
  for (int y = 0; y < surface->h; y++)
  {
    for (int x = 0; x < surface->w; x++)
    {
      Uint32 pixel = getPixel(surface, x, y);
      Uint8  r, g, b;
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      if (r > max)
        max = r;
    }
  }
  return max;
}

void imageNormalizeBrightness(SDL_Surface *surface)
{
  Uint8 m = maxColor(surface);
  for (int x = 0; x < surface->w; x++)
  {
    for (int y = 0; y < surface->h; y++)
    {
      Uint32 pixel = getPixel(surface, x, y);
      Uint8  r, g, b;
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      Uint8 new_color = 255.f - r * (255.f / m);
      putPixel(surface, x, y,
               SDL_MapRGB(surface->format, new_color, new_color, new_color));
    }
  }
}

void contrast(SDL_Surface *surface)
{
  int   n        = 10;
  float maxColor = 255;
  int   m        = maxColor / n;
  for (int x = 0; x < surface->w; x++)
  {
    for (int y = 0; y < surface->h; y++)
    {
      Uint32 pixel = getPixel(surface, x, y);
      Uint8  r, g, b;
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      for (int i = 0; i < n; i++)
      {
        if (r >= i * m && r <= (i + 1) * m)
        {
          Uint8 color = (i + 1) * m;
          putPixel(surface, x, y,
                   SDL_MapRGB(surface->format, color, color, color));
        }
      }
    }
  }
  // Invert the image for further processing
  invert(surface);
  // Normalize the brightness over the image
  imageNormalizeBrightness(surface);
}