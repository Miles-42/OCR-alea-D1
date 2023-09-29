#include "noise_reduction.h"

Uint8 medianFilter(Uint8 *neighbours)
{
  // Sort the neighbours
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 9; j++)
    {
      if (neighbours[i] > neighbours[j])
      {
        Uint8 tmp     = neighbours[i];
        neighbours[i] = neighbours[j];
        neighbours[j] = tmp;
      }
    }
  }
  // Return the median
  return neighbours[4];
}

Uint8 averageFilter(Uint8 *neighbours)
{
  // Filter using the pascal triangle (ponderated by 1/16)
  float filter[9] = {(1 / 16),  (2 / 16.), (1 / 16.), (2 / 16.), (4 / 16.),
                     (2 / 16.), (1 / 16.), (2 / 16.), (1 / 16.)};
  float result    = 0;
  for (int i = 0; i < 9; ++i)
  {
    result += neighbours[i] * filter[i];
  }
  return (Uint8)result;
}
void applyDenoise(SDL_Surface *surface, SDL_Surface *copy, Uint8 (*f)(Uint8 *))
{
  for (int y = 0; y < surface->h; y++)
  {
    for (int x = 0; x < surface->w; x++)
    {
      // Get the neighbours
      Uint8 *neighbours = getNeighbours(copy, x, y);
      // Apply a denoiser
      Uint8 value = (*f)(neighbours);
      // Set the pixel to the median
      Uint32 pixel = SDL_MapRGB(surface->format, value, value, value);
      putPixel(surface, x, y, pixel);
    }
  }
}

void denoise(SDL_Surface *surface)
{
  // Make a copy necessary to apply filter on the original image
  SDL_Surface *copy = copySurface(surface);
  // Apply the median filter
  applyDenoise(surface, copy, medianFilter);
  // Apply the average filter
  applyDenoise(surface, copy, averageFilter);

  SDL_FreeSurface(copy);
}