#include "grayscale.h"

void grayscale(SDL_Surface *surface)
{
  // Colorimetric (perceptual luminance-preserving) conversion to grayscale
  // https://en.wikipedia.org/wiki/Grayscale#Colorimetric_(perceptual_luminance-preserving)_conversion_to_grayscale

  Uint8  r, g, b;
  Uint32 pixel;

  for (int y = 0; y < surface->h; y++)
  {
    for (int x = 0; x < surface->w; x++)
    {
      pixel = getPixel(surface, x, y);
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      Uint8 gray = r * 0.2126 + g * 0.7152 + b * 0.0722;
      pixel      = SDL_MapRGB(surface->format, gray, gray, gray);
      putPixel(surface, x, y, pixel);
    }
  }
}
