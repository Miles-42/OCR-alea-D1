#include "lines.h"

void drawLine(SDL_Surface *surface, int xStart, int yStart, int xEnd, int yEnd,
              SDL_Color color, int width)
{
  Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);

  // draw a line from (xStart, yStart) to (xEnd, yEnd) of color (color) and
  // width (width) on each side of the line
  int dx = abs(xEnd - xStart);
  int dy = abs(yEnd - yStart);
  int x = xStart;
  int y = yStart;
  int n = 1 + dx + dy;
  int x_inc = (xEnd > xStart) ? 1 : -1;
  int y_inc = (yEnd > yStart) ? 1 : -1;
  int error = dx - dy;
  dx *= 2;
  dy *= 2;

  for (; n > 0; --n)
  {
    for (int i = -width; i <= width; i++)
    {
      for (int j = -width; j <= width; j++)
      {
        if (x + i >= 0 && x + i < surface->w && y + j >= 0 && y + j < surface->h)
        {
          putPixel(surface, x + i, y + j, pixel);
        }
      }
    }

    if (error > 0)
    {
      x += x_inc;
      error -= dy;
    }
    else
    {
      y += y_inc;
      error += dx;
    }
  }
}

void drawLines(SDL_Surface *surface, List *lines, SDL_Color color, int width)
{
  List *current = lines;
  while (current->next != NULL)
  {
    current = current->next;
    Line *l = (Line *)current->data;
    drawLine(surface, l->x1, l->y1, l->x2, l->y2, color, width);
  }
}

double line_angle(Line *l1, Line *l2)
{
  double x1 = l1->x2 - l1->x1;
  double y1 = l1->y2 - l1->y1;
  double x2 = l2->x2 - l2->x1;
  double y2 = l2->y2 - l2->y1;
  double dot = x1 * x2 + y1 * y2;
  double det = x1 * y2 - y1 * x2;
  double angle = atan2(det, dot);

  return angle;
}

double getAngle(Line *l)
{
  double x1 = l->x2 - l->x1;
  double y1 = l->y2 - l->y1;
  double angle = atan2(y1, x1);
  return angle;
}

double min(double a, double b)
{
  if (a < b)
  {
    return a;
  }
  return b;
}

double getRotationAngle(List *lines)
{
  // For all line that have an angle of 90 degrees approximately, get the angle
  // of l1

  List *current = lines;
  double maxAngle = 0;
  while (current->next != NULL)
  {
    current = current->next;
    Line *l1 = (Line *)current->data;
    List *current2 = lines;
    while (current2->next != NULL)
    {
      current2 = current2->next;
      Line *l2 = (Line *)current2->data;
      if (l1 == l2)
      {
        continue;
      }
      double lineAngle = line_angle(l1, l2);

      if (radiansToDegrees(lineAngle) < 90 + 10 && radiansToDegrees(lineAngle) > 90 - 10)
      {
        double angle = fmin(fabs(getAngle(l1)), fabs(getAngle(l2)));
        if (angle > maxAngle)
        {
          maxAngle = angle;
        }
      }
    }
  }
  return maxAngle;
}

void rotateLines(List *lines, double angle, int width, int height)
{
  // go through all lines
  // rotate their points from 0,0 by angle in radians
  List *current = lines;

  double cos_angle = cos(angle);
  double sin_angle = sin(angle);

  while (current->next != NULL)
  {
    current = current->next;
    Line *l = (Line *)current->data;
    int new_x, new_y;
    convertPoints(l->x1, l->y1, &new_x, &new_y, width, height, cos_angle,
                  sin_angle);
    l->x1 = new_x;
    l->y1 = new_y;
    convertPoints(l->x2, l->y2, &new_x, &new_y, width, height, cos_angle,
                  sin_angle);
    l->x2 = new_x;
    l->y2 = new_y;
  }
}

void getIntersection(Line l1, Line l2, int *x, int *y, int width, int height)
{
  double x1 = l1.x1;
  double x2 = l1.x2;
  double y1 = l1.y1;
  double y2 = l1.y2;

  double x3 = l2.x1;
  double x4 = l2.x2;
  double y3 = l2.y1;
  double y4 = l2.y2;

  double a1 = y2 - y1;
  double b1 = x1 - x2;
  double c1 = a1 * x1 + b1 * y1;

  double a2 = y4 - y3;
  double b2 = x3 - x4;
  double c2 = a2 * x3 + b2 * y3;

  double determinant = a1 * b2 - a2 * b1;
  if (determinant == 0)
  {
    // The lines are parallel
    *x = -1;
    *y = -1;
    return;
  }
  else
  {
    double a = (b2 * c1 - b1 * c2) / determinant;
    double b = (a1 * c2 - a2 * c1) / determinant;
    // test if the intersection is within the bounds of the image
    if (a < 0 || a > width || b < 0 || b > height)
    {
      *x = -1;
      *y = -1;
      return;
    }
    *x = (int)a;
    *y = (int)b;
  }
}

SudokuCell makeSquare(int x1, int y1, int x2, int y2, int x3, int y3, int x4,
                      int y4)
{
  // get the bottom left point
  int xBottomLeft = fmin(fmin(x1, x2), fmin(x3, x4));
  int yBottomLeft = fmin(fmin(y1, y2), fmin(y3, y4));
  // get the top right point
  int xTopRight = fmax(fmax(x1, x2), fmax(x3, x4));
  int yTopRight = fmax(fmax(y1, y2), fmax(y3, y4));
  // get the bottom right point
  int xBottomRight = fmax(fmax(x1, x2), fmax(x3, x4));
  int yBottomRight = fmin(fmin(y1, y2), fmin(y3, y4));
  // get the top left point
  int xTopLeft = fmin(fmin(x1, x2), fmin(x3, x4));
  int yTopLeft = fmax(fmax(y1, y2), fmax(y3, y4));

  SudokuCell cell = {
      .xBottomLeft = xBottomLeft,
      .yBottomLeft = yBottomLeft,
      .xBottomRight = xBottomRight,
      .yBottomRight = yBottomRight,
      .xTopLeft = xTopLeft,
      .yTopLeft = yTopLeft,
      .xTopRight = xTopRight,
      .yTopRight = yTopRight,

  };
  return cell;
}

List *squareDetection(List *lines, int width, int height)
{
  List *start = initList();
  List *squares = start;
  /*Debugging*/
  int count = 0;
  for (List *temp = lines; temp != NULL; temp = temp->next)
  {
    count++;
  }
  printf("List size: %d\n", count);

  for (List *n1 = lines->next; n1->next != NULL; n1 = n1->next)
  {
    for (List *n2 = n1->next; n2->next != NULL; n2 = n2->next)
    {
      if (n1 == n2)
        continue;

      Line *line1 = (Line *)n1->data;
      Line *line2 = (Line *)n2->data;
      // Get all line that actualLine have a intersection point with
      int intX, intY;
      getIntersection(*line1, *line2, &intX, &intY, width, height);
      if (intX == -1)
        continue;

      for (List *n3 = lines->next; n3->next != NULL; n3 = n3->next)
      {
        if (n2 == n3)
          continue;

        Line *line3 = (Line *)n3->data;
        int intX2, intY2;
        getIntersection(*line2, *line3, &intX2, &intY2, width, height);

        if (intX2 == -1)
          continue;

        for (List *n4 = n3->next; n4->next != NULL; n4 = n4->next)
        {
          if (n4 == n3)
            continue;

          Line *line4 = (Line *)n4->data;
          int intX3, intY3;
          getIntersection(*line3, *line4, &intX3, &intY3, width, height);
          if (intX3 == -1)
            continue;

          // DOES K have intersection with h

          int intX4, intY4;
          getIntersection(*line4, *line1, &intX4, &intY4, width, height);

          if (intX4 != -1)
          {
            double distance1 = sqrt(pow(intX - intX2, 2) + pow(intY - intY2, 2));

            double distance2 = sqrt(pow(intX2 - intX3, 2) + pow(intY2 - intY3, 2));

            double distance3 = sqrt(pow(intX3 - intX4, 2) + pow(intY3 - intY4, 2));

            double distance4 = sqrt(pow(intX4 - intX, 2) + pow(intY4 - intY, 2));

            if (distance1 < 10)
              continue;
            if (fabs(distance1 - distance2) > 5 || fabs(distance2 - distance3) > 5 || fabs(distance3 - distance4) > 5 || fabs(distance4 - distance1) > 5)
            {
              continue;
            }

            // test if any of the lines pointer is equal to another
            if (n1 == n2 || n1 == n3 || n1 == n4 || n2 == n3 || n2 == n4 || n3 == n4)
            {
              continue;
            }
            SudokuCell cell = makeSquare(intX, intY, intX2, intY2, intX3,
                                         intY3, intX4, intY4);
            SudokuCell *cellPtr = malloc(sizeof(SudokuCell));
            if (cellPtr == NULL)
            {
              printf("malloc failed\n");
            }
            *cellPtr = cell;
            listInsert(squares, cellPtr);
          }
        }
      }
    }
  }
  return start;
}

List *sortSquares(List *squares)
{
  // Sort the squares by the size of the square using bubble sort
  int swapped = 1;
  while (swapped)
  {
    swapped = 0;
    for (List *n1 = squares->next; n1->next != NULL; n1 = n1->next)
    {
      SudokuCell *cell1 = (SudokuCell *)n1->data;
      SudokuCell *cell2 = (SudokuCell *)n1->next->data;
      // get size of the square

      // int size1 = sqrt(pow(cell1->xBottomLeft - cell1->xBottomRight, 2)
      //                  + pow(cell1->yBottomLeft - cell1->yBottomRight, 2));
      // int size2 = sqrt(pow(cell2->xBottomLeft - cell2->xBottomRight, 2)
      //                  + pow(cell2->yBottomLeft - cell2->yBottomRight, 2));
      int size1 = abs(cell1->xBottomLeft - cell1->xBottomRight);
      int size2 = abs(cell2->xBottomLeft - cell2->xBottomRight);
      // printf("size1: %d, size2: %d\n", size1, size2);
      if (size1 > size2)
      {
        swapped = 1;
        void *temp = n1->data;
        n1->data = n1->next->data;
        n1->next->data = temp;
      }
    }
  }
  return squares;
}

void printSquares(List *squares)
{
  for (List *n1 = squares->next; n1->next != NULL; n1 = n1->next)
  {
    SudokuCell *cell1 = (SudokuCell *)n1->data;
    // get size of the square
    int size = cell1->xBottomLeft - cell1->xBottomRight;
    printf("size: %d\n", size);
  }
}

int getMedianSizeSquares(List *squares)
{
  // From a sorted list return the median size of the squares
  int size = listLength(squares->next);
  int median = size / 4;
  // int median = 10;
  int i = 0;
  for (List *n1 = squares->next; n1->next != NULL; n1 = n1->next)
  {
    if (i == median)
    {
      SudokuCell *cell1 = (SudokuCell *)n1->data;
      // get size of the square
      int size = abs(cell1->xBottomLeft - cell1->xBottomRight);
      return size;
    }
    i++;
  }
  return -1;
}
List *squareFilter(List *squares)
{
  printf("Squares before filter\n");
  List *sortedSquares = sortSquares(squares);
  // printSquares(sortedSquares);
  SudokuCell *cell = (SudokuCell *)sortedSquares->next->data;
  int cellSize = abs(cell->xBottomLeft - cell->xBottomRight);
  printf("min cell size: %d\n", cellSize);

  int medianSize = getMedianSizeSquares(sortedSquares);
  printf("Median size: %d\n", medianSize);

  List *filteredSquares = initList();
  // int   threshold       = 130;
  int threshold = medianSize;
  // int threshold = 80;

  int errorAcceptance = 0.2 * threshold;

  int upperThreshold = threshold + errorAcceptance;
  int lowerThreshold = threshold - errorAcceptance;

  printf("acceptance : (%d, %d)\n", lowerThreshold, upperThreshold);

  for (List *n1 = squares->next; n1->next != NULL; n1 = n1->next)
  {
    SudokuCell *cell1 = (SudokuCell *)n1->data;
    // get size of the square
    int size = sqrt(pow(cell1->xBottomLeft - cell1->xBottomRight, 2) + pow(cell1->yBottomLeft - cell1->yBottomRight, 2));
    if (size > lowerThreshold && size < upperThreshold)
      listInsert(filteredSquares, cell1);
  }
  freeList(squares);
  return filteredSquares;
}

SudokuCell selectSudoku(SDL_Surface *surface, List *squares)
{
  SDL_Color violet = {255, 0, 255, 255};
  // From all the squares selec the one that is the most on the left
  // Verify that there is 9 other squares under it and 9 other squares on his
  // right
  SudokuCell savedLeftMostCell = selectLeftMostCell(squares);
  // drawSquare(surface, &savedLeftMostCell, violet);
  SudokuCell verticalCell = savedLeftMostCell;
  int threshold = 25;

  // Verify that there is 9 other squares under it and 9 other squares on his
  // right
  int verticalStreak = 0;
  for (List *n = squares->next; n->next != NULL; n = n->next)
  {
    SudokuCell *cell = (SudokuCell *)n->data;
    if (abs(cell->xBottomLeft - verticalCell.xTopLeft) < threshold && abs(cell->yBottomLeft - verticalCell.yTopLeft) < threshold)
    {
      // drawSquare(surface, cell, colors[color]);
      verticalStreak++;
      verticalCell = *cell;
      n = squares->next;
      if (verticalStreak == 8)
      {
        printf("here\n");
        break;
      }
    }
  }
  printf("Vertical streak : %d\n", verticalStreak);

  SudokuCell horizontalCell = savedLeftMostCell;

  int horizontalStreak = 0;
  for (List *n = squares->next; n->next != NULL; n = n->next)
  {
    SudokuCell *cell = (SudokuCell *)n->data;
    if (abs(cell->xTopLeft - horizontalCell.xTopRight) < threshold && abs(cell->yTopLeft - horizontalCell.yTopRight) < threshold)
    {
      // drawSquare(surface, cell, colors[color]);
      horizontalStreak++;
      horizontalCell = *cell;
      n = squares->next;
      if (horizontalStreak == 8)
      {
        printf("here\n");
        break;
      }
    }
  }
  printf("Horizontal streak : %d\n", horizontalStreak);

  if (horizontalStreak < 8 && verticalStreak < 8)
    errx(1, "Not a sudoku");
  if (horizontalStreak == 8 && verticalStreak == 8)
  {

    SudokuCell cell = {
        .xTopLeft = verticalCell.xTopLeft,
        .yTopLeft = verticalCell.yTopLeft,
        .xTopRight = horizontalCell.xTopRight,
        .yTopRight = verticalCell.yTopRight,
        .xBottomRight = horizontalCell.xBottomRight,
        .yBottomRight = horizontalCell.yBottomRight,
        .xBottomLeft = savedLeftMostCell.xBottomLeft,
        .yBottomLeft = savedLeftMostCell.yBottomLeft,
    };
    drawSquare(surface, &cell, violet, 2);
    return cell;
  }

  if (verticalStreak < 8)
  {
    verticalCell = horizontalCell;
    verticalStreak = 0;
    for (List *n = squares->next; n->next != NULL; n = n->next)
    {
      SudokuCell *cell = (SudokuCell *)n->data;
      if (abs(cell->xBottomLeft - verticalCell.xTopLeft) < threshold && abs(cell->yBottomLeft - verticalCell.yTopLeft) < threshold)
      {
        // drawSquare(surface, cell, colors[color]);
        verticalStreak++;
        verticalCell = *cell;
        n = squares->next;
        if (verticalStreak == 8)
        {
          printf("here\n");
          break;
        }
      }
    }
    printf("Vertical streak : %d\n", verticalStreak);

    if (verticalStreak < 8)
      errx(1, "Not a sudoku");

    SudokuCell cell = {
        .xTopLeft = savedLeftMostCell.xTopLeft,
        .yTopLeft = verticalCell.yTopLeft,
        .xTopRight = horizontalCell.xTopRight,
        .yTopRight = verticalCell.yTopRight,
        .xBottomRight = horizontalCell.xBottomRight,
        .yBottomRight = horizontalCell.yBottomRight,
        .xBottomLeft = savedLeftMostCell.xBottomLeft,
        .yBottomLeft = savedLeftMostCell.yBottomLeft,
    };
    drawSquare(surface, &cell, violet, 2);
    return cell;
  }
  return savedLeftMostCell;
}

SudokuCell selectLeftMostCell(List *squares)
{
  int threshold = 15;

  SudokuCell *cell = (SudokuCell *)squares->next->data;
  SudokuCell selectedCell = *cell;
  // int         cpt          = 0;
  // Uint32      red          = SDL_MapRGB(surface->format, 255, 0, 0);
  for (List *n = squares->next; n->next != NULL; n = n->next)
  {
    SudokuCell *cell = (SudokuCell *)n->data;

    if ((cell->xBottomLeft <= selectedCell.xBottomLeft && abs(cell->yBottomLeft - selectedCell.yBottomLeft) < threshold) || (cell->yBottomLeft <= selectedCell.yBottomLeft && abs(cell->xBottomLeft - selectedCell.xBottomLeft) < threshold))
    {
      selectedCell = *cell;
    }
  }
  return selectedCell;
}
SudokuCell selectRightMostCell(List *squares)
{
  int threshold = 15;

  SudokuCell *cell = (SudokuCell *)squares->next->data;
  SudokuCell selectedCell = *cell;
  // int         cpt          = 0;
  // Uint32      red          = SDL_MapRGB(surface->format, 255, 0, 0);
  for (List *n = squares->next; n->next != NULL; n = n->next)
  {
    SudokuCell *cell = (SudokuCell *)n->data;

    if ((cell->xTopRight >= selectedCell.xTopRight && abs(cell->yTopRight - selectedCell.yTopRight) < threshold) || (cell->yTopRight >= selectedCell.yTopRight && abs(cell->xTopRight - selectedCell.xTopRight) < threshold))
    {
      selectedCell = *cell;
    }
  }
  return selectedCell;
}

void drawSquare(SDL_Surface *surface, SudokuCell *cell, SDL_Color color,
                int width)
{
  drawLine(surface, cell->xTopLeft, cell->yTopLeft, cell->xTopRight,
           cell->yTopRight, color, width);
  drawLine(surface, cell->xTopLeft, cell->yTopLeft, cell->xBottomLeft,
           cell->yBottomLeft, color, width);
  drawLine(surface, cell->xBottomLeft, cell->yBottomLeft, cell->xBottomRight,
           cell->yBottomRight, color, width);
  drawLine(surface, cell->xBottomRight, cell->yBottomRight, cell->xTopRight,
           cell->yTopRight, color, width);
}

void drawSquares(SDL_Surface *surface, List *squares, SDL_Color color)
{
  List *current = squares;
  while (current->next != NULL)
  {
    current = current->next;
    SudokuCell *cell = (SudokuCell *)current->data;
    if (cell == NULL)
    {
      printf("cell is null\n");
    }
    drawSquare(surface, cell, color, 1);
  }
}

void lineCleaning(List *list)
{
  // Remove all the elements line in the list that are close to each other
  // under a threshold
  int threshold = 80;
  List *current = list;
  while (current->next != NULL)
  {
    current = current->next;
    Line *line1 = (Line *)current->data;
    List *next = current->next;
    if (current == NULL || next == NULL)
    {
      break;
    }
    while (next->next != NULL)
    {
      next = next->next;
      Line *line2 = (Line *)next->data;
      if (abs(line1->x1 - line2->x1) < threshold && abs(line1->y1 - line2->y1) < threshold && abs(line1->x2 - line2->x2) < threshold && abs(line1->y2 - line2->y2) < threshold)
      {
        void *toRemove = next->data;
        if (next->next == NULL)
        {
          break;
        }
        next = next->next;

        listRemove(list, toRemove);
      }
    }
  }
}

void point(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  // put pixel in 10x10 square around x,y
  for (int i = -10; i < 10; i++)
  {
    for (int j = -10; j < 10; j++)
    {
      if (x + i >= 0 && x + i < surface->w && y + j >= 0 && y + j < surface->h)
      {
        putPixel(surface, x + i, y + j, pixel);
      }
    }
  }
}

void highlight(SDL_Surface *surface, List *lines)
{
  // find intersections between lines

  List *line1 = lines;
  List *line2 = lines;

  while (line1->next != NULL)
  {
    line1 = line1->next;
    Line *l1 = line1->data;
    line2 = lines;
    while (line2->next != NULL)
    {
      line2 = line2->next;
      Line *l2 = line2->data;
      double x1 = l1->x1;
      double x2 = l1->x2;
      double y1 = l1->y1;
      double y2 = l1->y2;

      double x3 = l2->x1;
      double x4 = l2->x2;
      double y3 = l2->y1;
      double y4 = l2->y2;

      double a1 = y2 - y1;
      double b1 = x1 - x2;
      double c1 = a1 * x1 + b1 * y1;

      double a2 = y4 - y3;
      double b2 = x3 - x4;
      double c2 = a2 * x3 + b2 * y3;

      double determinant = a1 * b2 - a2 * b1;
      if (determinant == 0)
      {
        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        continue;
      }
      else
      {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        Uint32 pixel = SDL_MapRGB(surface->format, 0, 0, 255);
        point(surface, (int)x, (int)y, pixel);
      }
    }
  }
}
