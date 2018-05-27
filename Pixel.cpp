#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include "Pixel.h"

Pixel null_pixel(0.0, 0.0, 0.0, 1.0);
Pixel black_pixel(0.0, 0.0, 0.0, 1.0);
Pixel red_pixel(1.0, 0.0, 0.0, 1.0);
Pixel green_pixel(0.0, 1.0, 0.0, 1.0);
Pixel blue_pixel(0.0, 0.0, 1.0, 1.0);
Pixel yellow_pixel(1.0, 1.0, 0.0, 1.0);
Pixel cyan_pixel(0.0, 1.0, 1.0, 1.0);
Pixel magenta_pixel(1.0, 0.0, 1.0, 1.0);
Pixel white_pixel(1.0, 1.0, 1.0, 1.0);

Pixel::
Pixel(void)
{
  // Initialize components to zero
  c[0] = 0;
  c[1] = 0;
  c[2] = 0;
  c[3] = 0;
}

Pixel::
Pixel(const Pixel& pixel)
{
  // Copy components
  c[0] = pixel.c[0];
  c[1] = pixel.c[1];
  c[2] = pixel.c[2];
  c[3] = pixel.c[3];
}

Pixel::
Pixel(double red, double green, double blue, double alpha)
{
  // Set components
  c[0] = red;
  c[1] = green;
  c[2] = blue;
  c[3] = alpha;
}

Pixel::
Pixel(const double rgba[4])
{
  // Set components
  c[0] = rgba[0];
  c[1] = rgba[1];
  c[2] = rgba[2];
  c[3] = rgba[3];
}

bool Pixel::
operator==(const Pixel& pixel) const
{
  // Return whether pixel is equal
  return (c[0] == pixel.c[0]) && (c[1] == pixel.c[1]) && (c[2] == pixel.c[2]) &&
         (c[3] == pixel.c[3]);
}

bool Pixel::
operator!=(const Pixel& pixel) const
{
  // Return whether pixel is not equal
  return (c[0] != pixel.c[0]) || (c[1] != pixel.c[1]) || (c[2] != pixel.c[2]) ||
         (c[3] != pixel.c[3]);
}

Pixel& Pixel::
operator=(const Pixel& pixel)
{
  // Copy components
  c[0] = pixel.c[0];
  c[1] = pixel.c[1];
  c[2] = pixel.c[2];
  c[3] = pixel.c[3];
  return *this;
}

Pixel& Pixel::
operator+=(const Pixel& pixel)
{
  c[0] += pixel.c[0];
  c[1] += pixel.c[1];
  c[2] += pixel.c[2];
  return *this;
}

Pixel& Pixel::
operator-=(const Pixel& pixel)
{
  c[0] -= pixel.c[0];
  c[1] -= pixel.c[1];
  c[2] -= pixel.c[2];
  return *this;
}

Pixel& Pixel::
operator*=(const Pixel& pixel)
{
  c[0] *= pixel.c[0];
  c[1] *= pixel.c[1];
  c[2] *= pixel.c[2];
  return *this;
}

Pixel& Pixel::
operator*=(double a)
{
  c[0] *= a;
  c[1] *= a;
  c[2] *= a;
  return *this;
}

Pixel& Pixel::
operator/=(double a)
{
  //  assert(!zero(a));
  c[0] /= a;
  c[1] /= a;
  c[2] /= a;
  return *this;
}
