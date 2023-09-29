#include "hough_transform.h"

void accToSurface(unsigned int *acc, SDL_Surface *surface, int width,
                  int height)
{
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      Uint8  value = acc[y * width + x];
      Uint32 color = SDL_MapRGB(surface->format, value, value, value);
      putPixel(surface, x, y, color);
    }
  }
}

void printAcc(unsigned int *acc, int width, int height)
{
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      printf("%d ", acc[y * width + x]);
    }
    printf("\n");
  }
}

void houghTransform(SDL_Surface *surface, List *list)
{
  // const float threshold = 0.55;
  // const float threshold = 0.5;
  const float threshold = 0.35;

  // Save the image dimensions
  const double width = surface->w, height = surface->h;
  // Calculate the diagonal of the image
  const double diagonal = sqrt(height * height + width * width);

  // Initialize the constant values for theta and rho
  const double maxTheta = 180.0, minTheta = 0.0;
  const double maxRho = diagonal, minRho = -diagonal;
  const double nbRho = 2 * diagonal, nbTheta = nbRho;

  // Create an array of rhos
  double  rhoStep = (maxRho - minRho) / nbRho;
  double *arrRhos = calloc(nbRho + 1, sizeof(double));
  int     index   = 0;
  for (double val = minRho; val <= maxRho && index < nbTheta;
       val += rhoStep, index++)
  {
    arrRhos[index] = val;
  }

  // Create an array of theta
  double  step      = (maxTheta - minTheta) / nbTheta;
  double *arrThetas = calloc(nbTheta + 1, sizeof(double));
  index             = 0;
  for (double val = minTheta; val <= maxTheta && index < nbTheta;
       val += step, index++)
  {
    arrThetas[index] = val;
  }
  // Create a save of cos and sin value for each theta, to optimize
  // performance.
  double *saveCos = calloc(nbTheta + 1, sizeof(double));
  double *saveSin = calloc(nbTheta + 1, sizeof(double));
  for (int theta = 0; theta < nbTheta; theta++)
  {
    // Convert each value of theta's array into radians
    arrThetas[theta] = degreesToRadians(arrThetas[theta]);

    // Save each cos(theta) and sin(theta) into their respective arrays
    saveCos[theta] = cos(arrThetas[theta]);
    saveSin[theta] = sin(arrThetas[theta]);
  }

  const int widthAcc = nbTheta + 1, heightAcc = nbRho + 1;

  unsigned int *accumulator
      = calloc(widthAcc * heightAcc, sizeof(unsigned int));

  // We intialize the accumulator with all the value
  // In the same time, we search for the max value in the accumulator
  unsigned int max = 0;
  double       rho;
  int          croppedRho;

  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      Uint32 pixel = getPixel(surface, x, y);
      Uint8  r, g, b;
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      if (r == 255)
      {
        for (int theta = 0; theta <= nbTheta; theta++)
        {
          rho        = x * saveCos[theta] + y * saveSin[theta];
          croppedRho = rho + diagonal;
          accumulator[croppedRho * widthAcc + theta]++;
          if (accumulator[croppedRho * widthAcc + theta] > max)
          {
            max = accumulator[croppedRho * widthAcc + theta];
          }
        }
      }
    }
  }

  free(saveCos);
  free(saveSin);

  double tempMaxTheta = 0.0;
  // unsigned int histogram[181] = {0};
  // unsigned int rounded_angle;

  List *lines = list;

  int prev       = accumulator[0];
  int prev_theta = 0, prev_rho = 0;
  int boolIsIncreasing = 1;
  int lineThreshold    = max * threshold;
  for (int theta = 0; theta <= nbTheta; theta++)
  {
    for (int rho = 0; rho <= nbRho; rho++)
    {
      int val = accumulator[rho * widthAcc + theta];
      if (val >= prev)
      {
        prev             = val;
        prev_rho         = rho;
        prev_theta       = theta;
        boolIsIncreasing = 1;
        continue;
      }
      else if (val < prev && boolIsIncreasing)
      {
        boolIsIncreasing = 0;
      }
      else if (val < prev)
      {
        prev       = val;
        prev_rho   = rho;
        prev_theta = theta;
        continue;
      }

      if (val >= lineThreshold)
      {
        double r = arrRhos[prev_rho], t = arrThetas[prev_theta];

        if (t > tempMaxTheta)
        {
          tempMaxTheta = t;
          // rounded_angle = (unsigned int)radiansToDegrees(t);
          // histogram[rounded_angle]++;
        }

        double c = cos(t), s = sin(t);
        // Calculate d0 point
        int d0X = (int)(c * r);
        int d0Y = (int)(s * r);
        // Calculate one point of the edge
        int xStart = d0X + (int)(diagonal * (-s));
        int yStart = d0Y + (int)(diagonal * c);

        // Calculate the other point of the edge
        int xEnd = d0X - (int)(diagonal * (-s));
        int yEnd = d0Y - (int)(diagonal * c);

        Line line = {.x1 = xStart, .y1 = yStart, .x2 = xEnd, .y2 = yEnd};

        Line *linePtr = malloc(sizeof(Line));
        *linePtr      = line;
        listInsert(lines, linePtr);
        // lines = lkAppend(lines, linePtr);
      }
    }
  }

  // SDL_Surface *surface2
  //     = SDL_CreateRGBSurface(0, nbTheta + 1, nbRho + 1, 32, 0, 0, 0, 0);
  // printAcc(accumulator, widthAcc, heightAcc);
  // accToSurface(accumulator, surface2, widthAcc, heightAcc);
  // saveSurface(surface2, "accumulator.jpg");

  free(arrThetas);
  free(arrRhos);
  free(accumulator);
}
